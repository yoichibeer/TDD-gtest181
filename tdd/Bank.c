#include "Bank.h"

#include <string.h>
#include <stdlib.h>

#include "detect_memory_leak.h"


struct Bank_ {
    Rate *rates;
    int size;
    int capacity;
};

struct Rate_ {
    const char *from;
    const char *to;
    int rateNum; // todo rateNumとかamountとか、いろいろbigdecimalに対応する。
};

static const Bank s_zeroBank = { 0 };
static const Rate s_zeroRate = { 0 };


Bank* createBank()
{
    Bank *bank = (Bank *)malloc(sizeof(Bank));
    if (bank == NULL)
        return NULL;
    *bank = s_zeroBank;
    return bank;
}

void disposeBank(Bank *bank)
{
    free(bank->rates);
    free(bank);
}

int rateByBank(Bank *bank, const char *from, const char *to)
{
    if (strcmp(from, to) == 0)
        return 1;

    // todo hashで検索したい。
    for (int i = 0; i < bank->size; ++i)
    {
        const Rate *rate = &bank->rates[i];
        if ((strcmp(rate->from, from) == 0)
            && (strcmp(rate->to, to) == 0))
        {
            return rate->rateNum;
        }
    }

    //assert(false);
    return 0; // 0 divide で落ちる
}

static bool addCapacity(Bank *bank)
{
    if (bank->capacity == bank->size)
    {
        bank->capacity += 10;
        Rate *rates = (Rate *)malloc(sizeof(Rate) * bank->capacity);
        if (rates == NULL)
            return false;

        for (int i = 0; i < bank->size; ++i)
        {
            rates[i] = bank->rates[i];
        }

        for (int i = bank->size; i < bank->capacity; ++i)
        {
#pragma warning(push)
#pragma warning( disable : 6386 ) // warning C6386: 'rates' への書き込み中にバッファー オーバーランが発生しました: 書き込み可能なサイズは 'sizeof(Rate)*(unsigned int)bank->capacity' バイトですが、'24' バイトを書き込む可能性があります。
            rates[i] = s_zeroRate;
#pragma warning(pop)
        }

        free(bank->rates);

        bank->rates = rates;

        return true;
    }
}


bool addRate(Bank *bank, const char *from, const char *to, int rate)
{
    if (!addCapacity(bank))
        return false;

    ++bank->size;
    bank->rates[bank->size - 1].from = from;
    bank->rates[bank->size - 1].to = to;
    bank->rates[bank->size - 1].rateNum = rate;

    return true;
}
