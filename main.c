#include <stdio.h>
#include "bignum.h"

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
    CHARARR nm1, nm2;
    char s;
    showmenu();
    menu();
    s = getchar();
    getchar();
   /* while(s != '0') {
        switch(s)
        {
            case '0':return;
            case '1':printf("\n             当前的运算格式为：A + B\n\n");
                     getnum(nm1, nm2);
                     printf("       运算结果为：        \n");
                     printf("%s\n", Result(nm1, nm2, addition));
                     printf("       运算时间为：%if ms\n", Time());
                     break;
            case '2':printf("\n             当前的运算格式为：A — B\n\n");
                     getnum(nm1, nm2);
                     printf("       运算结果为：        \n");
                     printf("%s\n", Result(nm1, nm2, subtraction));
                     printf("       运算时间为：%if ms\n", Time());
                     break;
            case '3':printf("\n             当前的运算格式为：A * B\n\n");
                     getnum(nm1, nm2);
                     printf("       运算结果为：        \n");
                     printf("%s\n", Result(nm1, nm2, multiplication));
                     printf("       运算时间为：%if ms\n", Time());
                     break;
            case '4':printf("\n             当前的运算格式为：A / B\n\n");
                     getnum(nm1, nm2);
                     printf("       运算结果为：        \n");
                     printf("%s\n", Result(nm1, nm2, dividsion));
                     printf("       运算时间为：%if ms\n", Time());
                     break;
            case '5':system("cls");
                     printf("\n             已清屏，回车继续运算！");
                     break;
            default:printf("\n              输入有误！按回车后重新输入选项");
                    break;
        }
        getchar();
        printf("\n\n");
        menu();
        s = getchar();
        getchar();

    }*/
    return 0;
}
