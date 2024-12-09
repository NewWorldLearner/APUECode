#include<stdio.h>
#include<sys/types.h>
#include<pwd.h>
#include<stdlib.h>


/*
* 练习passwd相关函数的使用
* /etc/passwd中的每一行都是用户信息记录，相关字段的含义可以man getpwnam，就可以查看passwd这个结构体中个字段的含义
* 掌握这5个函数的使用就可以了：getpwnam  getpwuid    getpwent    setpwent    endpwent
*
*/
int main(int argc, char **argv)
{
    struct passwd *pw = NULL;
    int uid = getuid();
    pw = getpwuid(uid);
    // pw_passwd这个字段表示用户的密码，但是这个密码肯定是不可见的，因此用一个字符串"x"来代替
    printf("%s\t%s\t%s\t%s\n", pw->pw_name, pw->pw_passwd, pw->pw_dir, pw->pw_shell);
    // 遍历passwd这个文件
    while((pw = getpwent()) != NULL)
    {
        printf("%s\t%s\t%s\t%s\n", pw->pw_name, pw->pw_passwd, pw->pw_dir, pw->pw_shell);
    }
    endpwent();
    return 0;
}