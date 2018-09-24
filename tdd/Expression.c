#include "Expression.h"

#include <string.h>
#include <stdlib.h>

#include "Bank.h"

#include "detect_memory_leak.h"


typedef Money *(*REDUCE_FUNC_TYPE)(Expression *expression, Bank *bank, const char* t);

// Base
struct Expression_ {
    REDUCE_FUNC_TYPE reduce;
};

// Derived from Base
struct Money_ {
    REDUCE_FUNC_TYPE reduce;

    int amount;
    const char *currency;
};

// Derived from Base
struct Sum_ {
    REDUCE_FUNC_TYPE reduce;

    Expression *augend;
    Expression *addend;
};
typedef struct Sum_ Sum;

// Derived from Base
struct Times_ {
    REDUCE_FUNC_TYPE reduce;

    Expression *multiplicand;
    int multiplier;
};
typedef struct Times_ Times;


static const Money s_zeroMoney = { 0 };
static const Sum s_zeroSum = { 0 };
static const Times s_zeroTimes = { 0 };


// todo 通貨はenumがいいのでは
static const char * const USD = "USD";
static const char * const CHF = "CHF";


static Money* createMoney(int amount, const char *currency);


static Money *reduceMoney(Expression *expression, Bank *bank, const char* to)
{
    Money *money = (Money *)expression;
    int rate = rateByBank(bank, currency(money), to);
    return createMoney(money->amount / rate, to);
}


static Money* createMoney(int amount, const char *currency)
{
    Money* money = (Money *)malloc(sizeof(Money));
    if (money == NULL)
        return NULL;
    *money = s_zeroMoney;
    money->amount = amount;
    money->currency = currency;
    money->reduce = reduceMoney;
    return money;
}


Money* createDollar(int amount)
{
    return createMoney(amount, USD);
}


Money* createFranc(int amount)
{
    return createMoney(amount, CHF);
}


void disposeMoney(Money *money)
{
    free(money);
}


void disposeExpression(Expression *expression)
{
    free(expression);
}


static Money *reduceTimes(Expression *expression, Bank *bank, const char* to)
{
    Times *times = (Times *)expression;
    Money *reduced = reduce(times->multiplicand, bank, to);
    const int amountNum = reduced->amount * times->multiplier;

    // 後始末
    disposeMoney(reduced);

    return createMoney(amountNum, to);
}


static Expression *createTimes(Expression *expression, int multiplier)
{
    Times* times = (Times *)malloc(sizeof(Times));
    if (times == NULL)
        return NULL;
    *times = s_zeroTimes;
    times->multiplicand = expression;
    times->multiplier = multiplier;
    times->reduce = reduceTimes;
    return (Expression *)times;
}


static Money *reduceSum(Expression *expression, Bank *bank, const char* to)
{
    Sum *sum = (Sum *)expression;
    Money *augend = reduce(sum->augend, bank, to);
    Money *addend = reduce(sum->addend, bank, to);
    const int amountNum = augend->amount + addend->amount;

    // 後始末
    disposeMoney(addend);
    disposeMoney(augend);

    return createMoney(amountNum, to);
}


static Expression *createSum(Expression *lhs, Expression *rhs)
{
    Sum* sum = (Sum *)malloc(sizeof(Sum));
    if (sum == NULL)
        return NULL;
    *sum = s_zeroSum;
    sum->augend = lhs;
    sum->addend = rhs;
    sum->reduce = reduceSum;
    return (Expression *)sum;
}


const char *currency(const Money *money)
{
    return money->currency;
}


Expression *plus(Expression *lhs, Expression *rhs)
{
    return createSum(lhs, rhs);
}

Expression *times(Expression *multiplicand, int multiplier)
{
    return createTimes(multiplicand, multiplier);
}

Money *reduce(Expression *expression, Bank *bank, const char* to)
{
    return expression->reduce(expression, bank, to);
}


// for debug / test
bool equals(const Money *lhs, const Money *rhs)
{
    // 通貨が違う場合は換算して一致するとしても異なると判断する。
    return (lhs->amount == rhs->amount) && (strcmp(lhs->currency, rhs->currency) == 0);
}

// for debug / test
Expression *augend(Expression *expression)
{
    return ((Sum *)expression)->augend;
}

// for debug / test
Expression *addend(Expression *expression)
{
    return ((Sum *)expression)->addend;
}
