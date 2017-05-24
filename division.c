#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"

void division(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes)
{
    BigNum m1 = *num1, m2 = *num2;
    int deta = m2.floatbits - m1.floatbits;
    MoveFloatPoint(&m1, m1.floatbits);
    MoveFloatPoint(&m2, m2.floatbits);
    InitBigNum(nRes);
  //  StartCount();
    if(IsZero(num1))
    {
      //  EndCount();
        return;
    }
    if(IsZero(num2))
    {
        nRes->sign = num1->sign;
        MakeInfinite(nRes);
      //  EndCount();
        return;
    }
    m1.sign = m2.sign = 0;
    while(m1.intbits != m2.intbits) {
        if(m1.intbits < m2.intbits)
        {
            MoveFloatPoint(&m1, 1);
            deta--;
        } else {
            MoveFloatPoint(&m2, 1);
            deta++;
        }
    }
    while(nRes->floatbits < (INT_BIT_MAX / Ret)) {
        int cmp = compare(&m1, &m2);
        int n = 10;
        BigNum mulres = {0}, subres = {0};
        if(cmp == -1)
        {
            MoveFloatPoint(&m1, 1);
            nRes->floatpart[nRes->floatbits++] = 0;
            continue;
        } else if(cmp == 0) {
            nRes->floatpart[nRes->floatbits++] = 1;
            break;
        }
        do {
            BigNum tmp = {0};
            tmp.intpart[0] = --n;
            tmp.intbits = 1;
            multiplication(&m2, &tmp, &mulres);
        } while((cmp = compare(&mulres, &m1)) == 1);
        nRes->floatpart[nRes->floatbits++] = n;
        if(cmp == 0)
            break;
        subtraction(&m1, &mulres, &subres);
        MoveFloatPoint(&subres, 1);
        m1 = subres;
    }
    MoveFloatPoint(nRes, 1);
    MoveFloatPoint(nRes, deta);
    if(num1->sign ^ num2->sign)
        nRes->sign = 1;
  //  EndCount();
}