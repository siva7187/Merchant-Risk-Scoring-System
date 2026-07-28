#include <stdio.h>
#include <string.h>
#include "merchant.h"

int main(void) {
    char again[8];

    printf("############################################\n");
    printf("   MERCHANT RISK SCORING SYSTEM (Terminal)\n");
    printf("############################################\n");

    do {
        Merchant m;
        memset(&m, 0, sizeof(m));

        readMerchantInput(&m);

        if (!validateMerchant(&m)) {
            printf("\nMerchant rejected due to invalid data. Skipping report.\n");
        } else {
            computeRatios(&m);
            computeSubRisks(&m);
            computeFinalScore(&m);
            classifyRisk(&m);
            printReport(&m);
        }

        printf("\nEvaluate another merchant? (y/n): ");
        if (fgets(again, sizeof(again), stdin) == NULL) break;
    } while (again[0] == 'y' || again[0] == 'Y');

    printf("\nSession ended. Goodbye.\n");
    return 0;
}
