#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
#include <windows.h>
#include <time.h>
*/

#define INT_BIT_MAX 1000
#define FLOAT_BIT_MAX 1000
#define Ret 100

typedef char CHARARR[1000];

#ifndef CONST
    #ifdef __cplusplus
        #define CONST const
    #else
        #define CONST
    #endif
#endif

typedef struct tagBigNum {
    int intbits;
    int floatbits;
    char infinite;
    char sign;
    char intpart[INT_BIT_MAX];
    char floatpart[FLOAT_BIT_MAX];
} BigNum;

typedef void (*PFNCALC)(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes);
/*
void StartCount();

void EndCount()

float Time();
*/
void InitBigNum(BigNum* Nm);

int CharLenByBigNum(CONST BigNum* Nm);

void CharToBigNum(CONST char *arr, BigNum* Nm);

void BigNumToChar(char *szBuf, CONST BigNum* Nm);

void AdjustBits(BigNum* Nm);

void MoveFloatPoint(BigNum* Nm, int deta);

void MakeInfinite(BigNum* Nm);

char *Result(CONST char *val1, CONST char *val2, PFNCALC pfnCalc);

int compare(CONST BigNum* num1, CONST BigNum* num2);

int IsZero(CONST BigNum* Nm);

void addition(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes);

void subtraction(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes);

void multiplication(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes);

void division(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes);

/*
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
*/
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

void addition(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes)
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
       // StartCount();
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
 //   EndCount()
}

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

    //    StartCount();

        if(!cmp)
        {
        //    EndCount()
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
  //  EndCount()
}

void multiplication(CONST BigNum* num1, CONST BigNum* num2, BigNum* nRes)
{
    BigNum m1 = *num1, m2 = *num2;
    BigNum sum = {0};
    int i, j;
    InitBigNum(nRes);
    MoveFloatPoint(&m1, m1.floatbits);
    MoveFloatPoint(&m2, m2.floatbits);
  //  StartCount();
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
  //  EndCount()
}

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
     //   EndCount()
        return;
    }
    if(IsZero(num2))
    {
        nRes->sign = num1->sign;
        MakeInfinite(nRes);
      //  EndCount()
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
 //   EndCount()
}



void showmenu()
{
    printf("\n");
    printf("\n");
    printf("               -- Welcome To You!--\n");
    printf("\n");
    printf("                本函数库实现了...\n");
    printf("                这些.............\n");
    printf("                用以..\n");
    printf("\n");
    printf("                -------------------\n");
}

void menu()
{
    printf("                ~~~ 可计算浮点数 ~~~\n");
    printf("                *                  *\n");
    printf("                *选择相应的选项执行操作\n");
    return;
}

void getnum(CHARARR numa, CHARARR numb)
{
    printf("    请输入数a：");
    scanf("%s", numa);
    printf("    请输入数b：");
    scanf("%s", numb);

    return;
}

int main(int argc, char* argv[])
{
	CHARARR num1;
    CHARARR num2;
    puts("Input two numbers,please:");
    while(scanf("%s%s",num1,num2)!=-1) {
        printf("The result of addition:\n%s\n",Result(num1,num2,addition));
        printf("The result of subtration:\n%s\n",Result(num1,num2,subtraction));
        printf("The result of multiplication:\n%s\n",Result(num1,num2,multiplication));
        printf("The result of divide:\n%s\n",Result(num1,num2,division));
        puts("Input two numbers,please:");
    }
    return 0;
}
