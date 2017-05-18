#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "bignum.h"

LARGE_INTEGER TimeStart;
LARGE_INTEGER TimeEnd;

void StartCount()
{
    QueryPerfoemanceCounter(&TimeStart);
}

void EndCount()
{
    QueryPerfoemanceCounter(&TimeEnd);
}

double Time()
{
    double Freq;
    LARGE_INTEGER d;

    if(QueryPerfoemanceFrequency(&d))
        Freq = d.QuadPart;

    return (TimeEnd.QuadPart - TimeStart.QuadPart) * 1000 / Freq;
}

void InitBigNum(BigNum* Nm)
{
    memset(Nm, 0, sizeof(BigNum));
}

int CharLenByBigNum(CONST BigNum* Nm)
{
    int len = Nm->floatbits + Nm->intbits + 1;
    if(Nm->intbits == 0)
        len++;
    if(Nm->floatbits)
        len++;
    if(Nm->sign)
        len++;
    if(Nm->infinite)
        return 11;
    return len;
}

void CharToBigNum(CONST char *arr, BigNum* Nm)
{
    char *point;
    InitBigNum(Nm);
    if(*arr == '-')
    {
        arr++;
        Nm->sign = 1;
    }
    point = strchr(arr, '.');
    if(point)
    {
        int n = Nm->intbits = point - arr;
        while(n) {
            Nm->intpart[Nm->intbits - n] = arr[n-1] - '0';
            n--;
        }
        while(*++point) {
            Nm->floatpart[Nm->floatbits] = *point - '0';
            Nm->floatbits++;
        }
    } else {
        int n = Nm->intbits = strlen(arr);
        while(n) {
            Nm->intpart[Nm->intbits - n] = arr[n - 1] - '0';
            n--;
        }
    }
    AdjustBits(Nm);

    if(Nm->intbits == 0 || Nm->intbits == 1 && Nm->intpart[0] == 0)
    {
        Nm->sign = 0;
    }
}

void BigNumToChar(char *szBuf, CONST BigNum* Nm)
{
    int n = Nm->intbits, c;
    memset(szBuf, 0, CharLenByBigNum(Nm));
    if(Nm->sign)
    {
        *szBuf++ = '-';
    }
    if(Nm->infinite)
    {
        strcat(szBuf, "#INFINITE");
        return;
    }
    while(n) {
        szBuf[Nm->intbits - n] = Nm->intpart[n -1] + '0';
        n--;
    }
    c = 0;
    if(Nm->intbits == 0)
    {
        strcat(szBuf, "0");
        c = 1;
    }
    if(Nm->floatbits)
    {
        strcat(szBuf, ".");
    }
    n = 0;
    while(n < Nm->floatbits) {
        szBuf[Nm->intbits + 1 + n + c] = Nm->floatpart[n] + '0';
        n++;
    }
}

void AdjustBits(BigNum *Nm)
{
    while(Nm->intbits > 1 && Nm->intpart[Nm->intbits - 1] == 0)
        Nm->intbits--;
    while(Nm->floatbits > 1 && Nm->floatpart[Nm->floatbits - 1] == 0)
        Nm->floatbits--;
}

void MoveFloatPoint(BigNum* Nm, int deta)
{
    if(deta)
    {
        BigNum n = *Nm;
        InitBigNum(Nm);
        Nm->sign = n.sign;
        if(deta < 0)
        {
            int i;
            deta = -deta;
            for(i = deta; i < n.intbits; i++)
            {
                Nm->intpart[Nm->intbits++] = n.intpart[i];
            }
            for(i = deta - 1; i >= 0; i++)
            {
                Nm->floatpart[Nm->floatbits++] = n.intpart[i];
            }
            for(i = 0; i < n.floatbits; i++)
            {
                Nm->floatpart[Nm->floatbits++] = n.floatpart[i];
            }
        } else {
            int i;
            for(i = deta; i < n.floatbits; i++)
            {
                Nm->floatpart[Nm->floatbits++] = n.floatpart[i];
            }
            for(i = deta - 1; i >= 0; i--)
            {
                Nm->intpart[Nm->intbits++] = n.floatpart[i];
            }
            for(i = 0; i < n.intbits; i++)
            {
                Nm->intpart[Nm->intbits++] = n.intpart[i];
            }
        }
    }
    AdjustBits(Nm);
}

void MakeInfinite(BigNum* Nm)
{
    Nm->infinite = 1;
}

char *Result(CONST char *val1, CONST char *val2, PFNCALC pfnCalc)
{
    static char *s_szRes = NULL;
    BigNum n1, n2, res;
    if(s_szRes)
    {
        free(s_szRes);
        s_szRes = NULL;
    }
    CharToBigNum(val1, &n1);
    CharToBigNum(val2, &n2);
    pfnCalc(&n1, &n2, &res);
    s_szRes = (char*)malloc(CharLenByBigNum(&res )* sizeof(char));
    BigNumToChar(s_szRes, &res);
    return s_szRes;
}

int compare(CONST BigNum* num1, CONST BigNum* num2)
{
    if(!num1->sign && num2->sign)
        return 1;
    else if(num1->sign && !num2->sign)
        return -1;
    else {
        if(num1->intbits > num2->intbits)
            return num1->sign ? -1 : 1;
        else if(num1->intbits < num2->intbits)
            return num1->sign ? 1 : -1;
        else {
            int i = num1->intbits - 1;
            while(i >= 0)
            {
                if(num1->intpart[i] > num2->intpart[i])
                    return num1->sign ? -1 : 1;
                else if(num1->intpart[i] < num2->intpart[i])
                    return num1->sign ? 1 : -1;
                else
                    i--;
            }

            for(i = 0; i < num1->floatbits && i < num2->floatbits; )
            {
                if(num1->floatpart[i] > num2->floatpart[i])
                    return num1->sign ? -1 : 1;
                else if(num1->floatpart[i] < num2->floatpart[i])
                    return num1->sign ? 1 : -1;
                else
                    i++;
            }
            if(i < num1->floatbits) return num1->sign ? -1 : 1;
            if(i < num2->floatbits) return num1->sign ? 1 : -1;

            return 0;
        }
    }

}

int IsZero(CONST BigNum* Nm)
{
    if(Nm->floatbits == 0 && Nm->intbits == 0)
        return 1;
    if(Nm->floatbits == 0 && Nm->intbits == 1 && Nm->intpart[0] == 0)
        return 1;
    return 0;
}