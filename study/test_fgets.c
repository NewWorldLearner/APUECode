#include<stdio.h>

/*
* 主要验证一下gets函数是一个不安全函数，当输入的字符串为"aaaaaa"时，str1字符串的值就被改成"aaaaa"了
* gets函数将输入存放到str2地址开始的内存中，当输入的字符串超过str2数组的大小时
* 由于栈是从上往下增长的，str2的下一个字节的地址便是str1，因此多余的字符串便会被用来覆盖住str1数组中的数据
* 更为严重的是，栈是从上往下长的，因此在这个输入的超长字符串可能会一直往上覆盖住函数栈帧中的其它地方的数据
* 这个可能会引发段错误
*/
int main(int argc, char **argv)
{
    char str1[] = "11111";
    char str2[1];
    printf("please input:");
    gets(str2);
    printf("str2:%s\n",str2);
    printf("str1:%s\n",str1);
    return 0;
}