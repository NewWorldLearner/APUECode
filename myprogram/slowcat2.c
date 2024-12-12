#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include <signal.h>

/*
* 实现slowcat，每秒钟从文件中读取一定的字符数
* 令牌桶实现——每秒钟可以读取一定的字节数，当停顿的时候，攒下应该读取的字节数
* 每秒钟增加1个token，token数量最大为100，每个token对应的cps为10
*
*/

#define CPS 10
#define BUFSIZE CPS
#define BURST 100

static volatile int token = 0;

void alrm_handler(int sign)
{
    alarm(1);
    token++;
    if (token > 100)
    {
        token = BURST;
    }
}

int main(int argc, char **argv)
{
    int sfd, dfd = 1;
    char buf[BUFSIZE];
    int len, ret, pos;

    if (argc < 2)
    {
        fprintf(stderr, "Usage error.......\n");
        return 0;
    }

    alarm(1);
    signal(SIGALRM, alrm_handler);

    do
    {
        sfd = open(argv[1], O_RDONLY);
        if (sfd < 0)
        {
            if (errno != EINTR)
            {
                perror("open()");
                return 0;
            }
        }
    } while (sfd < 0);
    while(1)
    {
        while(token <= 0)
            pause();
        token --;
        while ((len = read(sfd, buf, BUFSIZE)) < 0)
        {
            if (len < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                perror("read()");
                break;
            }
        }

        if (len == 0)
        {
            break;
        }

        pos = 0;
        while (len > 0)
        {
            // 这个地方为什么要这么写呢？是因为写入文件的时候，发生中断，那么只会写入一部分字符吗？
            ret = write(dfd, buf + pos, len);
            if (ret < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                perror("write()");
                return 0;
            }
            pos += ret;
            len -= ret;
        }
        sleep(1);
    }
    close(sfd);
    return 0;
}