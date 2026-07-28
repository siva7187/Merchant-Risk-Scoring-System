#include <string.h>
#include "merchant.h"

#define W_BUSINESS   0.20
#define W_CHARGEBACK 0.30
#define W_REFUND     0.20
#define W_FRAUD      0.20
#define W_AGE        0.05
#define W_VOLUME     0.05

void computeFinalScore(Merchant *m) {
    m->riskScore =
        m->businessRisk   * W_BUSINESS +
        m->chargebackRisk * W_CHARGEBACK +
        m->refundRisk      * W_REFUND +
        m->fraudRisk        * W_FRAUD +
        m->ageRisk           * W_AGE +
        m->volumeRisk        * W_VOLUME;
}

void classifyRisk(Merchant *m) {
    if (m->riskScore <= 35) {
        strcpy(m->riskLevel, "LOW");
    } else if (m->riskScore <= 70) {
        strcpy(m->riskLevel, "MEDIUM");
    } else {
        strcpy(m->riskLevel, "HIGH");
    }
}
