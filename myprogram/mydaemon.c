#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>
#include<string.h>
#include<errno.h>

/*
* 实现一个守护进程mydaemon，每秒钟往一个文件中写入一个数值
* 守护进程的编程规则:
* 1. 首先调用umask将文件模式创建屏蔽字设为0
* 2. 调用fork，然后使父进程退出
* 3. 调用setsid创建1个新会话
* 4. 将当前工作目录改为根目录
* 5. 关闭不再需要的文件描述符
* 6. 某些守护进程打开/dev/null使其具有文件描述符0、1、2
*/


#define FILENAME "/home/shy/APUECode/myprogram/out"

int daemonize()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        // perror("fork()");
        return -1;
    }
    // 父进程退出，让子进程成为孤儿进程，从而被init进程收养
    if (pid > 0)
    {
        exit(0);
    }
    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        // perror("open()");
        return -1;
    }

    // 守护进程不与终端进行交互
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
    {
        close(fd);
    }
    // 创建一个新的会话，并将当前进程变成新会话首进程
    setsid();

    umask(0);

    // 改变当前进程的工作目录，根目录是一定存在的，因此把工作目录改为根目录是比较稳妥的
    // 在这里我们已经把工作目录改为了根目录，因此后续打开或创建1个文件时，一定要使用绝对路径，如果使用相对路径，在是在根目录下创建或打开文件，会没有权限
    chdir("/");

    return 0;
}

int main(int argc, char **argv)
{
    // 第1个参数和第2个参数的作用如下：
    // Dec 10 21:08:38 localhost mydaemon[102047]: daemonize success!
    openlog("mydaemon", LOG_PID, LOG_DAEMON);
    if (daemonize() == -1)
    {
        // 注意，对于第2个参数，字符串中出现\n表示2个字符\和n，并非表示换行
        syslog(LOG_ERR, "LOG_ERR:daemonize failed!");
        exit(1);
    }
    else
    {
        // LOG_INFO以上等级的信息写入到/var/log/messages这个文件中，需要有权限才能打开这个文件
        syslog(LOG_INFO, "LOG_INFO:daemonize success!");
    }
    // 这里的文件路径一定要是绝对路径，因为工作目录已经被我们修改为根目录，而普通用户可能没有权限在根目录下创建文件
    FILE *fp = fopen(FILENAME, "w+");
    if (fp == NULL)
    {
        syslog(LOG_ERR, "LOG_ERR:fopen():%s", strerror(errno));
        return 0;
    }
    syslog(LOG_INFO, "LOG_INFO:%s was open", FILENAME);
    int i = 0;
    while(1)
    {
        fprintf(fp, "%d\n", i++);
        // 全缓冲模式下需要手动刷新
        fflush(fp);
        syslog(LOG_DEBUG, "LOG_DEBUG:%d is printed", i);
        sleep(1);
    }

    flcose(fp);
    closelog();
    return 0;
}