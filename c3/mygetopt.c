#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

/*
* 该程序的作用是解析命令行参数，如 ./mygetopt -a -b -c -h -f file.txt
* 选项的顺序不同，程序的执行结果有差异
*/

int main(int argc, char **argv)
{
    int opt=0;
    // getopt的第三个参数是由选项组成的字符串，如果某字符后面跟着':',则表示该选项后面有参数
    // 如果选项字符串以':'开头，则表示用户在没有给选项提供参数时，选项的参数为':'，如果不以':'开头的话，则不存在选项时，选项的参数默认为'?'
    // '-'如果要生效，则必须放在选项字符串的开头；而':'的作用要生效可以放在开头或者'-'后面
    while((opt = getopt(argc, argv, "-:abchf:")) != -1)
    {
        switch(opt)
        {
            case 1:
                // 识别到非选项参数时，getopt返回1，这时候optind指向的是下一个待解析的参数，因此optind-1指向的就是非选项参数在argv中的下标
                printf("non-option arguments:%s\n", argv[optind - 1]);
                break;
            case 'a':
            case 'b':
            case 'c':
                printf("option: %c\n", opt);
                break;
            case 'f':
                // f后面跟着':'，表示f这个选项有一个参数，optarg是一个全局变量，表示选项后面的参数
                printf("filename: %s\n", optarg);
                break;
            case ':':
                // 如果选项字符串以':'开头，则要求选项后面有参数而用户未提供时，则getopt返回':'
                printf("option needs a value\n");
                break;
            case '?':
                // 遇到无法识别的选项时，getopt返回'?'，并将该选项保存到全局变量optopt（整型变量）中
                // 或者如果选项字符串没有以':'开头，则要求选项后面有参数而用户未提供时，则getopt返回'?',并且还会打印报错信息
                // 总结一下，如果选项字符串不以':'开头，则遇到不认识的选项或者要求选项后面跟参数但用户未提供时，都统一认为选项错误，返回'?'，并且还会报错
                // 因此，选项字符串最好以':'开头
                printf("unknow option:%c\n", optopt);
        }
    }
    return 0;
}