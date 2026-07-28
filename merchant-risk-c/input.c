#include <stdio.h>
#include <string.h>
#include "merchant.h"

static int readInt(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF); /* flush */
            return value;
        }
        printf("Invalid number, try again.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF);
    }
}

void readMerchantInput(Merchant *m) {
    printf("\n===== MERCHANT INPUT =====\n");

    printf("Merchant Name: ");
    if (fgets(m->name, NAME_LEN, stdin) != NULL) {
        size_t len = strlen(m->name);
        if (len > 0 && m->name[len - 1] == '\n') m->name[len - 1] = '\0';
    }

    printf("\nSelect Business Type:\n");
    printf(" 0. Grocery\n 1. Clothing\n 2. Electronics\n 3. Travel\n 4. Gaming\n 5. Crypto\n");
    m->businessType = readInt("Choice (0-5): ");

    m->businessAge   = readInt("Business Age (months): ");
    m->transactions  = readInt("Monthly Transactions: ");
    m->chargebacks   = readInt("Chargebacks (count): ");
    m->refunds       = readInt("Refunds (count): ");
    m->fraudCases    = readInt("Fraud Cases (count): ");
}
