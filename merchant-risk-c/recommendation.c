#include <stdio.h>
#include <string.h>
#include "merchant.h"

void printRecommendations(const Merchant *m) {
    printf("\nRecommendation\n");
    if (strcmp(m->riskLevel, "LOW") == 0) {
        printf("  Approve Merchant\n");
        printf("  Quarterly Monitoring\n");
    } else if (strcmp(m->riskLevel, "MEDIUM") == 0) {
        printf("  Weekly Monitoring\n");
        printf("  Enhanced Verification\n");
        printf("  Monthly Review\n");
    } else { /* HIGH */
        printf("  Manual KYC\n");
        printf("  Daily Monitoring\n");
        printf("  Transaction Limit\n");
        printf("  Fraud Investigation\n");
        printf("  Manual Approval Required\n");
    }
}
