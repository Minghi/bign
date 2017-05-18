void addition(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes)
{
    InitBigNum(nRes);
    if(num1->sign ^ num2->sign)
    {
        BigNum n2 = *num2;
        n2.sign = num1->sign;
        subtraction(num1, &n2, nRes);
    } else {
        int maxfloatbits = num1->floatbits > num2->floatbits ? num1->floatbits : num2->floatbits;
        int addbit = 0;
        int i, j;
        StartCount();
        for(i = maxfloatbits - 1; i >= 0; i--)
        {
            int value = num1->floatpart[i] + num2->floatpart[i] + addbit;
            addbit = value / 10;
            nRes->floatpart[i] = value % 10;
        }
        nRes->floatbits = maxfloatbits;

        for(j = 0; j < num1->intbits || j < num2->intbits; j++)
        {
            int value = num1->intpart[j] + num2->intpart[j] + addbit;
            addbit = value / 10;
            nRes->intpart[j] = value % 10;
            nRes->intbits++;
        }
        if(addbit > 0)
        {
            nRes->intpart[j] = addbit;
            nRes->intbits++;
        }
        nRes->sign = num1->sign;
        AdjustBits(nRes);
    }
    EndCount()
}
