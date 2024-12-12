#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include <signal.h>
#include"mytbf.h"

/*
* 实现slowcat，每秒钟从文件中读取一定的字符数
* 令牌桶实现——每秒钟可以读取一定的字节数，当停顿的时候，攒下应该读取的字节数
* 每秒钟增加一次token，每次增加的token数量为cps，最大累计的token数量为burst
*
*/

#define CPS 10
#define BUFSIZE 1024
#define BURST 100

int main(int argc, char **argv)
{
    int sfd, dfd = 1;
    char buf[BUFSIZE];
    int len, ret, pos;
    mytbf_t *tbf;

    if (argc < 2)
    {
        fprintf(stderr, "Usage error.......\n");
        return 0;
    }

    tbf = mytbf_init(CPS, BURST);
    if (tbf < 0)
    {

    }

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
        int size = mytbf_fetchtoken(tbf, BUFSIZE);

        if (size < 0)
        {
            fprintf(stderr, "mytbf_fetchtoken():%s\n", strerror(-size));
            return 0;
        }

        while ((len = read(sfd, buf, size)) < 0)
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

        if (size - len > 0)
        {
            mytbf_returntoken(tbf, size - len);
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
    mytbf_destory(tbf);
    return 0;
}