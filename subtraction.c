#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"

void subtraction(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes)
{
    InitBigNum(nRes);
    if(num1->sign ^ num2->sign)
    {
        BigNum n2 = *num2;
        n2.sign = num1->sign;
        addition(num1, &n2, nRes);
    } else {
        int cmp = compare(num1, num2);
        int swapflag, i, maxfloatbits, subtractbit;

      //  StartCount();

        if(!cmp)
        {
          //  EndCount();
            return;
        }
        swapflag = num1->sign == 0 ? cmp == -1 : cmp == 1;
        if(swapflag)
        {
            CONST BigNum* n = num1;
            num1 = num2;
            num2 = n;
        }
        maxfloatbits = num1->floatbits > num2->floatbits ? num1->floatbits : num2->floatbits;
        subtractbit = 0;

        for(i = maxfloatbits - 1; i >= 0; i--)
        {
            if(num1->floatpart[i] - subtractbit < num2->floatpart[i])
            {
                int value = num1->floatpart[i] - num2->floatpart[i] - subtractbit + 10;
                subtractbit = 1;
                nRes->floatpart[i] = value;
            } else {
                int value = num1->floatpart[i] - num2->floatpart[i] - subtractbit;
                subtractbit = 0;
                nRes->floatpart[i] = value;
            }
        }
        nRes->floatbits = maxfloatbits;

        for(i = 0; i < num1->intbits || i < num2->intbits; i++)
        {
            if(num1->intpart[i] - subtractbit < num2->intpart[i])
            {
                int value = num1->intpart[i] - num2->intpart[i] - subtractbit + 10;
                subtractbit = 1;
                nRes->intpart[i] = value;
            } else {
                int value = num1->intpart[i] - num2->intpart[i] - subtractbit;
                subtractbit = 0;
                nRes->intpart[i] = value;
            }
            nRes->intbits++;
        }
        nRes->sign = swapflag ? !num1->sign : num1->sign;
        AdjustBits(nRes);
    }
  //  EndCount();
}