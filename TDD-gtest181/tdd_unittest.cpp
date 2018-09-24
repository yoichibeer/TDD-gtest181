#include "Bank.h"

#include "gtest/gtest.h"

#include "detect_memory_leak.h"


TEST(PART1_2_4, 掛け算)
{
    // 掛け算テスト

    Money *dollar = createDollar(5);
    Bank *bank = createBank();
    {
        Expression *expression = times((Expression *)dollar, 2);

        Money *expected = createDollar(10);
        Money *actual = reduce(expression, bank, "USD");
        EXPECT_TRUE(equals(expected, actual));

        //// 後始末
        disposeMoney(actual);
        disposeMoney(expected);
        disposeExpression(expression);
    }

    // dollarは不変を確認
    {
        Expression *expression = times((Expression *)dollar, 3);

        Money *expected = createDollar(15);
        Money *actual = reduce(expression, bank, "USD");
        EXPECT_TRUE(equals(expected, actual));

        // 後始末
        disposeMoney(actual);
        disposeMoney(expected);
        disposeExpression(expression);
    }

    // 後始末
    disposeBank(bank);
    disposeMoney(dollar);
}

TEST(PART3, 等しい)
{
    Money *dollar1 = createDollar(5);
    Money *dollar2 = createDollar(5);
    EXPECT_TRUE(equals(dollar1, dollar2));

    // 後始末
    disposeMoney(dollar2);
    disposeMoney(dollar1);
} 


TEST(PART3, 等しくない)
{
    Money *dollar1 = createDollar(5);
    Money *dollar2 = createDollar(10);
    EXPECT_FALSE(equals(dollar1, dollar2));

    // 後始末
    disposeMoney(dollar2);
    disposeMoney(dollar1);
}


TEST(PART7, 等しくない)
{
    Money *dollar = createDollar(5);
    Money *flanc = createFranc(5);
    EXPECT_FALSE(equals(dollar, flanc));

    // 後始末
    disposeMoney(flanc);
    disposeMoney(dollar);
}


TEST(PART9, 通貨)
{
    Money *dollar = createDollar(5);
    Money *flanc = createFranc(5);

    EXPECT_STREQ("USD", currency(dollar));
    EXPECT_STREQ("CHF", currency(flanc));

    // 後始末
    disposeMoney(flanc);
    disposeMoney(dollar);
}


TEST(PART12, シンプルな足し算)
{
    Money *five = createDollar(5);
    Expression *sum = plus((Expression *)five, (Expression *)five);
    Bank *bank = createBank();
    Money *result = reduce(sum, bank, "USD");
    Money *expectedDollar = createDollar(10);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeBank(bank);
    disposeExpression(sum);
    disposeMoney(five);
}


TEST(PART13, リターンさむ)
{
    Money *five = createDollar(5);
    Expression *sum = plus((Expression *)five, (Expression *)five);
    EXPECT_TRUE(equals(five, (Money *)augend(sum))); // augend はテスト用の関数。ExpressionをMoneyのキャストはまずい。
    EXPECT_TRUE(equals(five, (Money *)addend(sum))); // addend はテスト用の関数。ExpressionをMoneyのキャストはまずい。

    // 後始末
    disposeExpression(sum);
    disposeMoney(five);
}


TEST(PART13, reduceSum)
{
    Money *dollar1 = createDollar(3);
    Money *dollar2 = createDollar(4);
    Expression *sum = plus((Expression *)dollar1, (Expression *)dollar2);
    Bank *bank = createBank();
    Money *result = reduce(sum, bank, "USD");

    Money *expectedDollar = createDollar(7);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeBank(bank);
    disposeExpression(sum);
    disposeMoney(dollar2);
    disposeMoney(dollar1);
}


TEST(PART13, reduceMoney)
{
    Money *dollar = createDollar(1);
    Bank *bank = createBank();
    Money *result = reduce((Expression *)dollar, bank, "USD");

    Money *expectedDollar = createDollar(1);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeBank(bank);
    disposeMoney(dollar);
}


TEST(PART14, ドルとフランの換算)
{
    Bank *bank = createBank();
    addRate(bank, "CHF", "USD", 2);

    Money *flanc = createFranc(2);
    Money *result = reduce((Expression *)flanc, bank, "USD");

    Money *expectedDollar = createDollar(1);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeMoney(flanc);
    disposeBank(bank);
}


TEST(PART14, 同じ通貨のレート)
{
    Bank *bank = createBank();
    EXPECT_EQ(1, rateByBank(bank, "USD", "USD"));

    // 後始末
    disposeBank(bank);
}


TEST(PART15, ミックスの足し算)
{
    Money *fiveBucks = createDollar(5);
    Money *tenFranc = createFranc(10);
    Bank *bank = createBank();
    addRate(bank, "CHF", "USD", 2);
    Expression *sum = plus((Expression *)fiveBucks, (Expression *)tenFranc);
    Money *result = reduce(sum, bank, "USD");

    Money *expectedDollar = createDollar(10);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeExpression(sum);
    disposeBank(bank);
    disposeMoney(tenFranc);
    disposeMoney(fiveBucks);
}


TEST(PART16, Sumのplus)
{
    Money *fiveBucks = createDollar(5);
    Money *tenFranc = createFranc(10);

    Expression *sum1 = plus((Expression *)fiveBucks, (Expression *)tenFranc);
    Expression *sum2 = plus(sum1, (Expression *)fiveBucks);

    Bank *bank = createBank();
    addRate(bank, "CHF", "USD", 2);
    Money *result = reduce(sum2, bank, "USD");

    Money *expectedDollar = createDollar(15);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeBank(bank);
    disposeExpression(sum2);
    disposeExpression(sum1);
    disposeMoney(tenFranc);
    disposeMoney(fiveBucks);
}


TEST(PART16, Sumのtimes)
{
    Money *fiveBucks = createDollar(5);
    Money *tenFranc = createFranc(10);

    Expression *sum = plus((Expression *)fiveBucks, (Expression *)tenFranc);
    Expression *multiple = times(sum, 2);

    Bank *bank = createBank();
    addRate(bank, "CHF", "USD", 2);
    Money *result = reduce(multiple, bank, "USD");

    Money *expectedDollar = createDollar(20);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeBank(bank);
    disposeExpression(multiple);
    disposeExpression(sum);
    disposeMoney(tenFranc);
    disposeMoney(fiveBucks);
}


TEST(Part14, addRateをたくさんやってメモリリーク起こらないこととaddCapacityのポインタのコピーが動くこと)
{
    Money *fiveBucks = createDollar(5);
    Money *tenFranc = createFranc(10);

    Expression *sum = plus((Expression *)fiveBucks, (Expression *)tenFranc);
    Expression *multiple = times(sum, 3);

    Bank *bank = createBank();
    addRate(bank, "CHF", "USD", 2);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    addRate(bank, "CHF1", "USD1", 3);
    Money *result = reduce(multiple, bank, "USD");

    Money *expectedDollar = createDollar(30);
    EXPECT_TRUE(equals(expectedDollar, result));

    // 後始末
    disposeMoney(expectedDollar);
    disposeMoney(result);
    disposeBank(bank);
    disposeExpression(multiple);
    disposeExpression(sum);
    disposeMoney(tenFranc);
    disposeMoney(fiveBucks);
}


// 金額やレートが整数なのでうまくいかない
//TEST(番外編, 通貨に逆をいれる場合)
//{
//    Money *fiveBucks = createDollar(5);
//    Money *tenFranc = createFranc(10);
//
//    Expression *sum = plus((Expression *)fiveBucks, (Expression *)tenFranc);
//    Expression *multiple = times(sum, 3);
//
//    Bank *bank = createBank();
//
//    addRate(bank, "CHF", "USD", 2);
//
//    //addRate(bank, "USD", "CHF", 0.5);
//    addRate(bank, "USD", "CHF", 1); // 本当は0.5だが現在は整数だけ対応。また、ここで1ではなくて0にすると0 divideで落ちる。
//
//    Money *result = reduce(multiple, bank, "CHF");
//
//    //Money *expectedDollar = createFranc(45);
//    Money *expectedDollar = createFranc(60);
//
//    EXPECT_TRUE(equals(expectedDollar, result));
//
//    // 後始末
//    disposeMoney(expectedDollar);
//    disposeMoney(result);
//    disposeBank(bank);
//    disposeExpression(multiple);
//    disposeExpression(sum);
//    disposeMoney(tenFranc);
//    disposeMoney(fiveBucks);
//}
