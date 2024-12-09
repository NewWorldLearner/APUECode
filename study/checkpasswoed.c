#include<stdio.h>
#include<stdlib.h>
#include <shadow.h>
#include <unistd.h>
#include <crypt.h>

#define _XOPEN_SOURCE
/*
* 实现密码校验功能，输入用户的密码时，通过原始密码获得加盐后的密码，并读取shadow中的相应记录的sp_pwdp字段来进行对比，从而可知用户密码正确与否
* 掌握getpass函数、crypt函数
*
*/


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage error......\n");
        return 0;
    }

    //关闭终端回显，并读取输入内容
    char * input_passwd = getpass("PassWord:");
    struct spwd *shadow_line = getspnam(argv[1]);
    // 第1个参数是原始密码，第2个参数是字符串"$%s$%s$"，%s表示1个字符串
    char *crypted = crypt(input_passwd, shadow_line->sp_pwdp);
    if (strcmp(shadow_line->sp_pwdp, crypted) == 0)
    {
        printf("ok\n");
    }
    else
    {
        printf("fasle");
    }
    return 0;
}