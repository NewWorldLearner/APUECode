#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

/*
* 实现mytree命令: ./mytree pathname
* 递归遍历一个目录下面的所有文件
*
*
*
*/

void mytree(char *dir, int depth)
{
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "cant't open directory %s\n", dir);
        return;
    }
    // 进入到指定的目录下面，然后对于指定的目录才可以使用相对路径来访问该目录下面的文件
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        // 把.和..排除以后，下面应该是形似树的深度优先搜索遍历，或者说形似树的先序遍历(先访问根，再访问根的孩子节点)
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            {
                continue;;
            }
            // 指定第1个%s对应的是空字符串，又指定其输出宽度，那么就会左补空格，使得第1个%s输出的是空格组成的字符串，*用来动态指定输出宽度
            printf("%*s%s/\n", depth, "", entry->d_name);
            mytree(entry->d_name, depth + 4);
        }
        else
        {
            printf("%*s%s\n", depth, "", entry->d_name);
        }
    }
    // 当前目录下面的所有文件都访问完之后，然后就回到上一层目录继续遍历其它文件
    chdir("..");
    closedir(dp);
}

int main(int argc, char **argv)
{
    char *topdir = ".";
    if (argc >= 2)
    {
        topdir = argv[1];
    }
    mytree(topdir, 0);
    return 0;
}

