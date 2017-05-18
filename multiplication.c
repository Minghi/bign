void multiplication(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes)
{
    BigNum m1 = *num1, m2 = *num2;
    BigNum sum = {0};
    int i, j;
    InitBigNum(nRes);
    MoveFloatPoint(&m1, m1, floatbits);
    MoveFloatPoint(&m2, m2, floatbits);
    StartCount();
    for(i = 0; i < m2.intbits; i++)
    {
        BigNum tmp = {0};
        int addbit = 0;
        tmp.intbits = m1.intbits + i;
        for(j = 0; j < m1.intbits; j++)
        {
            int value =  m2.intpart[i] * m1.intpart[j] + addbit;
            addbit = value / 10;
            tmp.intpart[j + i] = value % 10;
        }
        if(addbit)
        {
            tmp.intpart[j + i] = addbit;
            tmp.intbits++;
        }
        addition(&sum, &tmp, nRes);
        sum = *nRes;
    }
    *nRes = sum;
    MoveFloatPoint(nRes, -(num1->floatbits + num2->floatbits));

    if(num1->sign ^ num2->sign)
        nRes->sign = 1;
    EndCount()
}
