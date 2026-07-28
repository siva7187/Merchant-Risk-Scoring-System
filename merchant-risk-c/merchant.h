#ifndef MERCHANT_H
#define MERCHANT_H

#define NAME_LEN 50
#define LEVEL_LEN 20
#define MAX_FACTORS 6

/* Business type codes */
typedef enum {
    BIZ_GROCERY = 0,
    BIZ_CLOTHING,
    BIZ_ELECTRONICS,
    BIZ_TRAVEL,
    BIZ_GAMING,
    BIZ_CRYPTO,
    BIZ_COUNT
} BusinessType;

typedef struct {
    char name[NAME_LEN];
    int businessType;      /* index into BusinessType */
    int businessAge;       /* months */
    int transactions;
    int chargebacks;
    int refunds;
    int fraudCases;

    double chargebackRatio; /* percent */
    double refundRatio;     /* percent */

    /* individual 0-100 risk sub-scores */
    double businessRisk;
    double ageRisk;
    double chargebackRisk;
    double refundRisk;
    double fraudRisk;
    double volumeRisk;

    double riskScore;       /* final weighted 0-100 */
    char riskLevel[LEVEL_LEN];
} Merchant;

typedef struct {
    const char *label;
    double risk;
    double weight;
    double contribution;
} RiskFactor;

/* input.c */
void readMerchantInput(Merchant *m);

/* validation.c */
int validateMerchant(const Merchant *m);

/* risk_engine.c */
void computeRatios(Merchant *m);
void computeSubRisks(Merchant *m);

/* score_engine.c */
void computeFinalScore(Merchant *m);
void classifyRisk(Merchant *m);

/* recommendation.c */
void printRecommendations(const Merchant *m);

/* report.c */
void printReport(const Merchant *m);
void buildFactorTable(const Merchant *m, RiskFactor factors[MAX_FACTORS]);
void printTopRiskFactors(const Merchant *m);
void printAlerts(const Merchant *m);

/* utils.c */
const char *businessTypeName(int type);
void toUpperStr(char *s);

#endif
