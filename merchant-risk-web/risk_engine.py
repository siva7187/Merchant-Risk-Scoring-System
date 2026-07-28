"""Core Merchant Risk Scoring logic - pure functions, no framework dependencies."""

BUSINESS_TYPES = ["Grocery", "Clothing", "Electronics", "Travel", "Gaming", "Crypto"]
BUSINESS_RISK = {"Grocery": 10, "Clothing": 20, "Electronics": 40, "Travel": 50, "Gaming": 80, "Crypto": 100}

WEIGHTS = {
    "business": 0.20,
    "chargeback": 0.30,
    "refund": 0.20,
    "fraud": 0.20,
    "age": 0.05,
    "volume": 0.05,
}


class ValidationError(Exception):
    pass


def validate(data: dict):
    errs = []
    if data.get("businessType") not in BUSINESS_TYPES:
        errs.append("businessType must be one of: " + ", ".join(BUSINESS_TYPES))
    for field in ("businessAge", "transactions", "chargebacks", "refunds", "fraudCases"):
        val = data.get(field)
        if not isinstance(val, (int, float)):
            errs.append(f"{field} must be a number")
    if errs:
        raise ValidationError("; ".join(errs))

    if data["transactions"] <= 0:
        errs.append("transactions must be > 0")
    if data["businessAge"] < 0:
        errs.append("businessAge cannot be negative")
    if data["fraudCases"] < 0:
        errs.append("fraudCases cannot be negative")
    if data["chargebacks"] < 0 or data["refunds"] < 0:
        errs.append("chargebacks/refunds cannot be negative")
    if data["chargebacks"] > data["transactions"]:
        errs.append("chargebacks cannot exceed transactions")
    if data["refunds"] > data["transactions"]:
        errs.append("refunds cannot exceed transactions")

    if errs:
        raise ValidationError("; ".join(errs))


def age_risk(months):
    if months > 36: return 10
    if months >= 12: return 30
    if months >= 6: return 60
    return 100


def chargeback_risk(ratio):
    if ratio < 1: return 10
    if ratio < 2: return 30
    if ratio < 4: return 60
    return 100


def refund_risk(ratio):
    if ratio < 2: return 10
    if ratio < 5: return 30
    if ratio < 10: return 60
    return 100


def fraud_risk(cases):
    if cases <= 0: return 0
    if cases == 1: return 30
    if cases == 2: return 60
    return 100


def volume_risk(tx):
    if tx < 100: return 20
    if tx <= 5000: return 10
    if tx <= 10000: return 30
    return 50


def classify(score):
    if score <= 35: return "LOW"
    if score <= 70: return "MEDIUM"
    return "HIGH"


def evaluate(data: dict) -> dict:
    """Takes raw merchant input dict, returns full scored merchant record."""
    validate(data)

    transactions = data["transactions"]
    chargebacks = data["chargebacks"]
    refunds = data["refunds"]
    fraud_cases = data["fraudCases"]
    age = data["businessAge"]
    biz_type = data["businessType"]

    chargeback_ratio = round(100.0 * chargebacks / transactions, 2)
    refund_ratio = round(100.0 * refunds / transactions, 2)

    sub_risks = {
        "Business Type Risk": BUSINESS_RISK[biz_type],
        "Chargeback Ratio Risk": chargeback_risk(chargeback_ratio),
        "Refund Ratio Risk": refund_risk(refund_ratio),
        "Fraud History Risk": fraud_risk(fraud_cases),
        "Merchant Age Risk": age_risk(age),
        "Transaction Volume Risk": volume_risk(transactions),
    }
    weight_map = {
        "Business Type Risk": WEIGHTS["business"],
        "Chargeback Ratio Risk": WEIGHTS["chargeback"],
        "Refund Ratio Risk": WEIGHTS["refund"],
        "Fraud History Risk": WEIGHTS["fraud"],
        "Merchant Age Risk": WEIGHTS["age"],
        "Transaction Volume Risk": WEIGHTS["volume"],
    }

    factors = []
    final_score = 0.0
    for label, risk in sub_risks.items():
        weight = weight_map[label]
        contribution = round(risk * weight, 2)
        final_score += contribution
        factors.append({"label": label, "risk": risk, "weight": weight, "contribution": contribution})

    final_score = round(final_score, 2)
    risk_level = classify(final_score)

    factors_sorted = sorted(factors, key=lambda f: f["contribution"], reverse=True)
    top_factors = [f["label"] for f in factors_sorted[:3]]

    alerts = []
    if chargeback_ratio > 4:
        alerts.append(f"High Chargeback Ratio ({chargeback_ratio}%)")
    if refund_ratio > 10:
        alerts.append(f"Too Many Refunds ({refund_ratio}%)")
    if age < 6:
        alerts.append(f"New Merchant ({age} months)")
    if fraud_cases > 0:
        alerts.append(f"Previous Fraud Cases ({fraud_cases})")
    if final_score > 70:
        alerts.append(f"Overall High Risk ({final_score})")

    if risk_level == "LOW":
        recommendations = ["Approve Merchant", "Quarterly Monitoring"]
    elif risk_level == "MEDIUM":
        recommendations = ["Weekly Monitoring", "Enhanced Verification", "Monthly Review"]
    else:
        recommendations = ["Manual KYC", "Daily Monitoring", "Transaction Limit",
                            "Fraud Investigation", "Manual Approval Required"]

    return {
        "name": data["name"],
        "businessType": biz_type,
        "businessAge": age,
        "transactions": transactions,
        "chargebacks": chargebacks,
        "refunds": refunds,
        "fraudCases": fraud_cases,
        "chargebackRatio": chargeback_ratio,
        "refundRatio": refund_ratio,
        "factors": factors,
        "riskScore": final_score,
        "riskLevel": risk_level,
        "topFactors": top_factors,
        "alerts": alerts,
        "recommendations": recommendations,
    }
