#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define FILENAME "out"

/*
* 作用：每隔1秒钟向指定文件中写入行号和当前时间
* 值得注意的是，输出文件不是终端，因此需要主动刷新输出缓冲区
*
*
*
*/
int main(int argc, char **argv)
{
    FILE* fp = fopen(FILENAME, "a+");
    if (fp == NULL)
    {
        perror("fopen()");
    }

    char buff[1024] = {0};
    int count = 0;
    while(fgets(buff, 1024, fp) != NULL)
    {
        count++;
    }

    time_t stamp;
    struct tm *tm1;
    while(1)
    {
        time(&stamp);
        tm1 = localtime(&stamp);
        fprintf(fp, "%-4d\t %d-%d-%d %d:%d:%d\n",++count, tm1->tm_year + 1900, tm1->tm_mon + 1, tm1->tm_yday, tm1->tm_hour, tm1->tm_min, tm1->tm_sec);
        // 需要刷新缓冲区，标准输入输出设备是行缓冲模式，其它文件默认都是全缓冲模式，因此需要主动刷新缓冲区
        fflush(fp);
        sleep(1);
    }
    fclose(fp);

    return 0;
}