#include<stdio.h>
#include<time.h>


int main(int argc, char **argv)
{
    time_t stamp = time(&stamp);
    struct tm *tm = localtime(&stamp);
    char buff[1024] = {'\0'};
    // strftime会自动添加换行符，因此需要在格式字符串后面添加换行符
    strftime(buff, 1024, "Now date:%Y-%m-%d %H:%M:%S", tm);
    puts(buff);

    tm->tm_yday += 100;

    // mktime函数会自动将tm结构体中不合理的数值给转化为合理的数值这是该函数的副作用
    // 因此这里只要将天数加100即可,这里无需关注返回值
    mktime(tm);
    strftime(buff, 1024, "After 100 days:%Y-%m-%d:%H:%M:%S", tm);
    puts(buff);
    return 0;
}