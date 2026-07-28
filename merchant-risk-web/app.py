from flask import Flask, request, jsonify, render_template, redirect, url_for
from itertools import count
from datetime import datetime
import risk_engine as engine

app = Flask(__name__)

MERCHANTS = {}
_id_counter = count(1)


def _store(record: dict) -> dict:
    mid = next(_id_counter)
    record["id"] = mid
    record["createdAt"] = datetime.utcnow().isoformat() + "Z"
    MERCHANTS[mid] = record
    return record


# ---------- JSON API ----------

@app.post("/api/merchants")
def api_create_merchant():
    payload = request.get_json(force=True, silent=True) or {}
    try:
        result = engine.evaluate(payload)
    except engine.ValidationError as e:
        return jsonify({"error": str(e)}), 400
    record = _store(result)
    return jsonify(record), 201


@app.get("/api/merchants")
def api_list_merchants():
    items = sorted(MERCHANTS.values(), key=lambda m: m["riskScore"], reverse=True)
    return jsonify(items)


@app.get("/api/merchants/<int:mid>")
def api_get_merchant(mid):
    m = MERCHANTS.get(mid)
    if not m:
        return jsonify({"error": "not found"}), 404
    return jsonify(m)


@app.get("/api/alerts")
def api_alerts():
    items = [m for m in MERCHANTS.values() if m["alerts"]]
    items.sort(key=lambda m: m["riskScore"], reverse=True)
    return jsonify(items)


@app.get("/api/business-types")
def api_business_types():
    return jsonify(engine.BUSINESS_TYPES)


# ---------- HTML pages ----------

@app.get("/")
def dashboard():
    items = sorted(MERCHANTS.values(), key=lambda m: m["riskScore"], reverse=True)
    counts = {"LOW": 0, "MEDIUM": 0, "HIGH": 0}
    alert_count = 0
    for m in items:
        counts[m["riskLevel"]] += 1
        if m["alerts"]:
            alert_count += 1
    return render_template("dashboard.html", merchants=items, counts=counts,
                            alert_count=alert_count, total=len(items))


@app.get("/add")
def add_form():
    return render_template("add.html", business_types=engine.BUSINESS_TYPES)


@app.post("/add")
def add_submit():
    form = request.form
    payload = {
        "name": form.get("name", "").strip() or "Unnamed Merchant",
        "businessType": form.get("businessType"),
        "businessAge": int(form.get("businessAge", 0) or 0),
        "transactions": int(form.get("transactions", 0) or 0),
        "chargebacks": int(form.get("chargebacks", 0) or 0),
        "refunds": int(form.get("refunds", 0) or 0),
        "fraudCases": int(form.get("fraudCases", 0) or 0),
    }
    try:
        result = engine.evaluate(payload)
    except engine.ValidationError as e:
        return render_template("add.html", business_types=engine.BUSINESS_TYPES,
                                error=str(e), form_values=payload), 400
    record = _store(result)
    return redirect(url_for("merchant_detail", mid=record["id"]))


@app.get("/merchant/<int:mid>")
def merchant_detail(mid):
    m = MERCHANTS.get(mid)
    if not m:
        return render_template("not_found.html", mid=mid), 404
    return render_template("detail.html", m=m)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5050, debug=True)
