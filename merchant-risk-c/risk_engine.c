#include "merchant.h"

static const double bizRisk[BIZ_COUNT] = {
    10, 20, 40, 50, 80, 100 /* Grocery..Crypto */
};

void computeRatios(Merchant *m) {
    m->chargebackRatio = (100.0 * m->chargebacks) / m->transactions;
    m->refundRatio      = (100.0 * m->refunds) / m->transactions;
}

static double ageRiskOf(int age) {
    if (age > 36) return 10;
    if (age >= 12) return 30;
    if (age >= 6)  return 60;
    return 100; /* <6 */
}

static double chargebackRiskOf(double ratio) {
    if (ratio < 1)  return 10;
    if (ratio < 2)  return 30;
    if (ratio < 4)  return 60;
    return 100;
}

static double refundRiskOf(double ratio) {
    if (ratio < 2)  return 10;
    if (ratio < 5)  return 30;
    if (ratio < 10) return 60;
    return 100;
}

static double fraudRiskOf(int cases) {
    if (cases <= 0) return 0;
    if (cases == 1) return 30;
    if (cases == 2) return 60;
    return 100; /* >=3 */
}

static double volumeRiskOf(int tx) {
    if (tx < 100)   return 20;
    if (tx <= 5000) return 10;
    if (tx <= 10000) return 30;
    return 50;
}

void computeSubRisks(Merchant *m) {
    m->businessRisk   = bizRisk[m->businessType];
    m->ageRisk         = ageRiskOf(m->businessAge);
    m->chargebackRisk = chargebackRiskOf(m->chargebackRatio);
    m->refundRisk      = refundRiskOf(m->refundRatio);
    m->fraudRisk        = fraudRiskOf(m->fraudCases);
    m->volumeRisk       = volumeRiskOf(m->transactions);
}
