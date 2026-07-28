#include <stdio.h>
#include "merchant.h"

/* Returns 1 if valid, 0 if invalid (prints reason). */
int validateMerchant(const Merchant *m) {
    if (m->transactions <= 0) {
        printf("Validation Error: Transactions must be > 0.\n");
        return 0;
    }
    if (m->businessAge < 0) {
        printf("Validation Error: Business age cannot be negative.\n");
        return 0;
    }
    if (m->fraudCases < 0) {
        printf("Validation Error: Fraud cases cannot be negative.\n");
        return 0;
    }
    if (m->chargebacks < 0 || m->refunds < 0) {
        printf("Validation Error: Chargebacks/Refunds cannot be negative.\n");
        return 0;
    }
    if (m->chargebacks > m->transactions) {
        printf("Validation Error: Chargebacks cannot exceed transactions.\n");
        return 0;
    }
    if (m->refunds > m->transactions) {
        printf("Validation Error: Refunds cannot exceed transactions.\n");
        return 0;
    }
    if (m->businessType < 0 || m->businessType >= BIZ_COUNT) {
        printf("Validation Error: Invalid business type selected.\n");
        return 0;
    }
    return 1;
}
