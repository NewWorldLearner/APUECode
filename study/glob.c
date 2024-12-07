#include<stdio.h>
#include<glob.h>

/*
* 练习glob函数的使用，glob函数的作用是用来匹配一个目录下面的文件名？因此可以用来实现目录扫描的作用？
* int glob(const char *pattern, int flags, int (*errfunc) (const char *epath, int eerrno), glob_t *pglob);
* void globfree(glob_t *pglob);
* typedef struct {
*               size_t   gl_pathc;
*               char   **gl_pathv;
*               size_t   gl_offs;
*          } glob_t;
* 第1个参数是模式串，一般来说就是含有通配符的字符串，第2个参数是特殊要求，这里我们只学习glob函数最简单的使用，因此传入0
* 第3个参数表示匹配字符串出错时要调用什么函数来进行处理，我们可以传入NULL，第4个参数是用来保存匹配到的结果——最后还应该记得释放相应的内存
*/

#define PAT "/etc/a*.conf"

#if 0
static int errfunc(const char*errpath, int eerror)
{
    puts(errpath);
    fprintf(stderr, "ERROR MSG:%s\n", strerror(eerror));
}
#endif


int main(int argc, char **argv)
{
    glob_t globres;
    int i;
    int err;
    err = glob(PAT, 0, NULL, &globres);
    if (err)
    {
        printf("Error code = %d\n", err);
        return 0;
    }
    for (int i = 0; i < globres.gl_pathc; i++)
    {
        puts(globres.gl_pathv[i]);
    }
    globfree(&globres);

    return 0;
}