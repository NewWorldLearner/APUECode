#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<glob.h>
#include<string.h>
#include<stdlib.h>

// 为什么已经包含了头文件还是必须要对该函数进行声明，否则就会有隐式声明的警告
int lstat(const char *restrict path, struct stat *restrict buf);

/*
* 实现du命令，目前先不考虑权限的问题
* ./mydu file
*
*
*/

int path_noloop(const char *path)
{
    char *pos;
    pos = strrchr(path, '/');
    if (pos == NULL)
    {
        // 需要包含对应头文件，否则会有警告
        // #include<stdlib.h>
        exit(1);
    }

    if (strcmp(pos + 1, ".") == 0 || strcmp(pos + 1, "..") == 0)
    {
        return 0;
    }

    return 1;
}

// 返回一个目录及其子文件所占的磁盘扇区数
int64_t mydu(const char *path)
{
    struct stat statres;
    if (lstat(path, &statres) == -1)
    {
        perror("lstat");
    }
    if (!S_ISDIR(statres.st_mode))
    {
        return statres.st_blocks;
    }

    char nextpath[1024];
    glob_t globres;
    strncpy(nextpath, path, 1024);
    strncat(nextpath, "/*", 1024);
    if (glob(nextpath, 0, NULL, &globres) == -1)
    {
        perror("glob");
    }

    strncpy(nextpath, path, 1024);
    strncat(nextpath, "/.*", 1024);
    // 注意这里第2次应该使用追加
    if (glob(nextpath, GLOB_APPEND, NULL, &globres) == -1)
    {
        perror("glob");
    }

    int sum = 0;
    for (int i = 0; i < globres.gl_pathc; i++)
    {
        // 在树状结构中不存在回路，因此可以直接使用递归，但很明显目录结构并不是树状结构，我们需要排除.和..这两个目录
        if (path_noloop(globres.gl_pathv[i]))
        {
            sum += mydu(globres.gl_pathv[i]);
        }

    }
    // 如果传进来的是一个目录，上面已经计算了该目录下面的各个文件或目录的大小，但是我们仍然需要这个目录本身的大小
    sum += statres.st_blocks;

    globfree(&globres);
    return sum;

}

int main(int argc, char **argv)
{
    printf("%d\n", mydu(argv[1]) / 2);
    return 0;
}