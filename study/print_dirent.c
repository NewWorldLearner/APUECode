#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include <dirent.h>

/*
* ./print_dirent filepath
* 传入文件路径，打印该路径下面的所有目录项(包括隐藏文件)
*
*
*/

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage error......");
        return 0;
    }
    DIR * dp;
    struct dirent *entry;
    dp = opendir(argv[1]);
    if (dp == NULL)
    {
        printf("open directory error\n");
        return 0;
    }
    printf("inode\tfile type\tfile name\tdirectory offset\tfile size\n");
    while ((entry = readdir(dp)) != NULL)
    {
        // 着重掌握name、ino这两个字段吧，因为其它字段并非POSIX.1标准，注意off并非数组下标，而是目录流中的偏移量
        printf("%u\t%c\t%s\t%u\t%u\n", entry->d_ino, entry->d_type,entry->d_name, entry->d_off, entry->d_reclen);
    }
    closedir(dp);
    return 0;
}