#include<stdio.h>
#include<string.h>
#include<glob.h>
#include <unistd.h>
#include <fcntl.h>

/*
* 实现一个myshell程序
*
*
*/
#define DELIMS " \t\n"


struct cmd_st
{
    glob_t globres;
};

void prompt()
{
    printf("[mysh-0.1$]");
}

void parse(char *line, struct cmd_st *res)
{
    char *tok;
    int i = 0;
    while(1)
    {
        // strsep在遇到分隔符时，会将分隔符变成'\0'，然后返回'\0'前面的那个单词的首地址
        tok = strsep(&line, DELIMS);
        if (tok == NULL)
        {
            break;
        }
        if (tok[0] == '\0')
        {
            continue;
        }
        glob(tok, GLOB_NOCHECK | GLOB_APPEND * i, NULL, &(res->globres));
        i = 1;
    }
}


int main(int argc, char **argv)
{
    char *linebuf = NULL;
    size_t linebuff_size = 0;
    struct cmd_st cmd;
    while(1)
    {
        prompt();
        if (getline(&linebuf, &linebuff_size, stdin) < 0)
        {
            break;
        }
        parse(linebuf, &cmd);

        if (0)
        {
            // 内部命令
        }
        else
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork()");
            }
            if (pid == 0)
            {
                execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
                perror("execvp()");
                return 0;
            }
            else
            {
                wait(NULL);
            }
        }

    }
    return 0;
}