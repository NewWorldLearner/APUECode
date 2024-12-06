#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

/*
* ./make_file_hole file.txt n(个字节)
* 先将lseek定位到文件file.txt末尾，然后从文件末尾制造指定大小为n的文件空洞，单位为字节
* 
*/
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage error......\n");
        return 0;
    }
    // 以append方式打开文件时，则lseek不会生效，这是为什么呢？
    // 猜测一下，以append方式打开文件时，每次写入文件时都会通过文件的大小来确定偏移量的值
    // 这就导致虽然以lseek来将文件偏移量定位到文件末尾之后，但是每次写入文件时，文件偏移量又被定位到文件末尾，这就导致lseek不生效
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
    int n = atoi(argv[2]);
    int cur = lseek(fd, 0, SEEK_END);
    printf("current offset:%d\n", cur);
    int res = lseek(fd, n, SEEK_END);
    if (res == -1)
    {
        perror("lseek error");
        return 0;
    }
    // 向文件末尾写入一个零字符
    write(fd, "", 1);
    printf("current file size:%d Bytes\n", res + 1);
    return 0;
}