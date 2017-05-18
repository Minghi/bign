/*
 * bignum.h
 */

#define INT_BIT_MAX 1000
#define FLOAT_BIT_MAX 1000
#define Ret 100

typedef char CHARARR[1000];

#ifdef CONST
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

void StartCount();

void EndCount()

float Time();

void InitBigNum(BigNum* Nm);

int CharLenByBigNum(CONST BigNum* Nm);

void CharToBigNum(CONST char *arr, BigNum* Nm);

void BigNumToChar(char *szBuf, CONST BigNum* Nm);

void AdjustBits(BigNum* Nm);

void MoveFloatPoint(BigNum* Nm, int deta);

void MakeInfinite(BigNum* Nm);

char *Result(CONST char *val1, CONST char *val2, PENCALC pfnCalc);

int compare(CONST BigNum* num1, CONST BigNum* num2);

int IsZero(CONST BigNum* Nm);

void addition(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes);

void subtraction(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes);

void multiplication(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes);

void division(CONST BigNum* num1, CONST BigNum* num2, CONST BigNum* nRes);

