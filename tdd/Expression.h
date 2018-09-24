#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


    // typedef
    typedef struct Expression_ Expression;
    typedef struct Money_ Money;
    typedef struct Bank_ Bank;


    // 生成
    Money *createDollar(int amount);
    Money *createFranc(int amount);


    // 破棄
    void disposeMoney(Money *money);
    void disposeExpression(Expression *expression);

    // Money
    const char *currency(const Money *money);


    // Expression
    Expression *times(Expression *multiplicand, int multiplier);
    Expression *plus(Expression *lhs, Expression *rhs);


    // Expression to Money
    Money *reduce(Expression *expression, Bank *bank, const char* to);


    // for debug / test
    bool equals(const Money *lhs, const Money *rhs);

    // for debug / test
    Expression *augend(Expression *expression);
    Expression *addend(Expression *expression);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* EXPRESSION_H */
