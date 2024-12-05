#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<getopt.h>
#include<stdbool.h>


#define BUFFSIZE 1024

/*
* 实现mytee命令， -a表示追加，选项中出现-a时，无论何时何地，表示对所有文件的写操作都是追加
* mytee的参数可以是多个文件
*
*/


int main(int argc, char **argv)
{
    char buff[1024] = {'\0'};
    int n = 0;
    int opt = 0;
    int fd[1024] = {0};
    int count = 0;
    bool is_append = false;
    while ((opt = getopt(argc, argv, "-a")) != -1)
    {
        switch (opt)
        {
            case 1:
                // 在这个地方值得注意的是，如果我们我们打开文件的时候不使用追加方式，那么文件偏移量在文件打开的时候就被设置为0了
                // 即使后续我们使用fcntl将文件设置为追加方式也无济于事，因为文件偏移量已经被设置为0了
                // 因此我们使用追加方式打开文件，如果命令中不含-a选项，那么我们就把文件内容清空
                fd[count] = open(argv[optind-1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd[count] == -1)
                {
                    perror("open error");
                }
                count++;
                break;
            case 'a':
                is_append = true;
                break;
            default:
                break;
        }
    }
    if (!is_append)
    {
        for (int i =0; i < count; i++)
        {
            ftruncate(fd[i], 0);
        }
    }
    while ((n = read(0, buff, BUFFSIZE)) >0)
    {
        write(1, buff, n);
        for (int i =0; i < count; i++)
        {
            write(fd[i], buff, n);
        }   
    }
    return 0;
}