#include<stdio.h>
#include <sys/time.h>
#include <time.h>

/*
* 练习了如下几个时间函数的使用,注意4种时间类型 time_t、timeval、timespec、tm
* time 获取时间戳
* gettimeofday 获取时间戳+微秒，相关函数settimeofday
* clock_gettime 获取时间戳+纳秒，相关函数clock_settime
* localtime 将时间戳转为tm结构体
* ctime 将时间戳转为字符串类型
* asctime 将tm类型的时间转为字符串类型
* strftime 将tm类型的时间转化为格式化的字符串时间
*/

int main(int argc, char **argv)
{
    struct timeval tv1;
    struct timeval tv2;
    // gettimeofday的第2个参数一般设为NULL
    gettimeofday(&tv1, NULL);
    gettimeofday(&tv2, NULL);

    // 获取时间戳，time成功时返回的也是时间戳
    time_t t1;
    time(&t1);

    printf("call gettimeofday: %d seconds, %d useconds\n", tv1.tv_sec, tv1.tv_usec);
    printf("call gettimeofday: %d seconds, %d useconds\n", tv2.tv_sec, tv2.tv_usec);
    printf("call time: %d seconds\n", t1);

    // 把时间戳转换为tm结构体
    struct tm *tm1;
    tm1 = localtime(&t1);
    printf("call localtime: %d-%d-%d %d:%d:%d\n", tm1->tm_year + 1900, tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min, tm1->tm_sec);

    // 把时间类型转换为字符串类型
    printf("call ctime: %s", ctime(&t1));
    printf("call asctime:%s", asctime(tm1));

    // 把tm类型的时间转化为格式化的字符串
    char buff[1024] = {0};
    strftime(buff, 1024, "%Y-%m-%d %H:%M:%S\n", tm1);
    printf("call strftime: %s", buff);

    // // #include<linux/time.h>
    // struct timespec ts1;
    // struct timespec ts2;
    // clock_gettime(CLOCK_REALTIME, &ts1);
    // clock_gettime(CLOCK_REALTIME, &ts2);
    // printf("%d seconds, %d nseconds\n", ts1.tv_sec, ts1.tv_nsec);
    // printf("%d seconds, %d nseconds\n", ts2.tv_sec, ts2.tv_nsec);
    return 0;
}