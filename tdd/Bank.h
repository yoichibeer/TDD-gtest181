#ifndef PART1X_H
#define PART1X_H

#include "Expression.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    typedef struct Bank_ Bank;
    typedef struct Rate_ Rate;

    Bank* createBank();

    void disposeBank(Bank *bank);

    bool addRate(Bank *bank, const char *from, const char *to, int rate);

    int rateByBank(Bank *bank, const char *from, const char *to);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* PART1X_H */
