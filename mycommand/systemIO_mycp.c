#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFSIZE 1024

// 使用系统io来实现cp命令


int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr,"Usgae Error......\n");
        return 0;
    }
    char buff[BUFFSIZE];
    // 不指定权限时，默认的权限是多少？
    int fd1 = open(argv[1], O_RDONLY, 0644);
    if (fd1 == -1)
    {
        perror("open error");
        return 0;
    }
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (fd2 == -1)
    {
        perror("open error");
        return 0;
    }
    int n = 0;
    while ((n = read(fd1, buff, BUFFSIZE)) > 0)
    {
        if(n = write(fd2, buff, n) != n)
        {
            perror("write error");
        }
    }
    close(fd1);
    close(fd2);
    return 0;
}