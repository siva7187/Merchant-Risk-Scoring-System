#include <ctype.h>
#include "merchant.h"

static const char *bizNames[BIZ_COUNT] = {
    "Grocery", "Clothing", "Electronics", "Travel", "Gaming", "Crypto"
};

const char *businessTypeName(int type) {
    if (type < 0 || type >= BIZ_COUNT) return "Unknown";
    return bizNames[type];
}

void toUpperStr(char *s) {
    for (; *s; ++s) *s = (char)toupper((unsigned char)*s);
}
