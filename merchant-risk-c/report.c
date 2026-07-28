#include <stdio.h>
#include <string.h>
#include "merchant.h"

void buildFactorTable(const Merchant *m, RiskFactor factors[MAX_FACTORS]) {
    factors[0] = (RiskFactor){"Business Type Risk",      m->businessRisk,   0.20, m->businessRisk   * 0.20};
    factors[1] = (RiskFactor){"Chargeback Ratio Risk",   m->chargebackRisk, 0.30, m->chargebackRisk * 0.30};
    factors[2] = (RiskFactor){"Refund Ratio Risk",       m->refundRisk,     0.20, m->refundRisk     * 0.20};
    factors[3] = (RiskFactor){"Fraud History Risk",      m->fraudRisk,      0.20, m->fraudRisk       * 0.20};
    factors[4] = (RiskFactor){"Merchant Age Risk",       m->ageRisk,         0.05, m->ageRisk          * 0.05};
    factors[5] = (RiskFactor){"Transaction Volume Risk", m->volumeRisk,      0.05, m->volumeRisk       * 0.05};
}

void printTopRiskFactors(const Merchant *m) {
    RiskFactor factors[MAX_FACTORS];
    buildFactorTable(m, factors);

    /* sort by contribution descending (simple bubble sort, N=6) */
    for (int i = 0; i < MAX_FACTORS - 1; i++) {
        for (int j = 0; j < MAX_FACTORS - 1 - i; j++) {
            if (factors[j].contribution < factors[j + 1].contribution) {
                RiskFactor tmp = factors[j];
                factors[j] = factors[j + 1];
                factors[j + 1] = tmp;
            }
        }
    }

    printf("\nTop Risk Factors\n");
    for (int i = 0; i < 3; i++) {
        printf("  %d. %s (contribution: %.1f)\n", i + 1, factors[i].label, factors[i].contribution);
    }
}

void printAlerts(const Merchant *m) {
    int any = 0;
    printf("\nAlerts\n");
    if (m->chargebackRatio > 4) { printf("  [!] High Chargeback Ratio (%.1f%%)\n", m->chargebackRatio); any = 1; }
    if (m->refundRatio > 10)     { printf("  [!] Too Many Refunds (%.1f%%)\n", m->refundRatio); any = 1; }
    if (m->businessAge < 6)      { printf("  [!] New Merchant (%d months)\n", m->businessAge); any = 1; }
    if (m->fraudCases > 0)       { printf("  [!] Previous Fraud Cases (%d)\n", m->fraudCases); any = 1; }
    if (m->riskScore > 70)       { printf("  [!] Overall High Risk (%.1f)\n", m->riskScore); any = 1; }
    if (!any) printf("  None\n");
}

void printReport(const Merchant *m) {
    RiskFactor factors[MAX_FACTORS];
    buildFactorTable(m, factors);

    printf("\n==========================================\n");
    printf("MERCHANT RISK REPORT\n");
    printf("==========================================\n");
    printf("Merchant Name       : %s\n", m->name);
    printf("Business Type       : %s\n", businessTypeName(m->businessType));
    printf("Business Age        : %d Months\n", m->businessAge);
    printf("Transactions        : %d\n", m->transactions);
    printf("Chargebacks         : %d\n", m->chargebacks);
    printf("Refunds             : %d\n", m->refunds);
    printf("Fraud Cases         : %d\n", m->fraudCases);
    printf("------------------------------------------\n");
    printf("Chargeback Ratio    : %.2f%%\n", m->chargebackRatio);
    printf("Refund Ratio        : %.2f%%\n", m->refundRatio);
    printf("------------------------------------------\n");
    printf("%-28s %6s %8s %8s\n", "Factor", "Risk", "Weight", "Contrib");
    for (int i = 0; i < MAX_FACTORS; i++) {
        printf("%-28s %6.1f %7.0f%% %8.2f\n",
               factors[i].label, factors[i].risk, factors[i].weight * 100, factors[i].contribution);
    }
    printf("------------------------------------------\n");
    printf("Final Score              : %.2f\n", m->riskScore);
    printf("Risk Level               : %s\n", m->riskLevel);
    printf("------------------------------------------\n");

    printTopRiskFactors(m);
    printAlerts(m);
    printRecommendations(m);

    printf("==========================================\n");
}
