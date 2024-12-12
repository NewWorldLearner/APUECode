#include<stdio.h>
#include<stdlib.h>

#include"mytbf.h"
#include<errno.h>
#include<signal.h>
#include<string.h>

// 手册上为了方便阅读才定义的这个类型，因此我们要手动定义这个类型
typedef void (*sighandler_t)(int);

static struct mytbf_st* job[MYTBF_MAX];
static inited = 0;
static sighandler_t alrm_handler_save;

/*
* 每秒钟增加1次token，每次增加的token数量为cps，token最大累计数量为burst
*
*/
struct mytbf_st
{
    int cps; // count per second
    int burst;
    int token;
    int pos;
};

static void alrm_handler(int s)
{
    int i = 0;
    alarm(1);
    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
            {
                job[i]->token = job[i]->burst;
            }
        }
    }
}

void module_unload()
{
    signal(SIGALRM, alrm_handler_save);
    alarm(0);
    int i = 0;
    for (i = 0; i< MYTBF_MAX; i++)
    {
        free(job[i]);
    }
}

static void module_load()
{
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);

    atexit(module_unload);
}

int get_free_pos()
{
    int i;
    for (i = 0; i < MYTBF_MAX; i++)
    {
        if (job[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

mytbf_t *mytbf_init(int cps, int burst)
{
    struct mytbf_st *me;

    if (!inited)
    {
        module_load();
        inited = 1;
    }

    me = malloc(sizeof(*me));
    int pos = get_free_pos();
    if (pos < 0)
    {
        return NULL;
    }
    if (me == NULL)
    {
        return NULL;
    }

    me->token = 0;
    me->cps = cps;
    me->burst = burst;
    me->pos = pos;
    job[pos] = me;
    return me;
}

int min(int m, int n)
{
    if (m <= n)
    {
        return m;
    }
    return n;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;
    if (size <= 0)
    {
        return -EINVAL;
    }

    while(me->token <= 0)
    {
        pause();
    }

    int n = min(me->token, size);

    me->token -=n;
    return n;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;
    if (size <= 0)
    {
        return -EINVAL;
    }

    me->token += size;
    if (me->token > me->burst)
    {
        me->token = me->burst;
    }

    return size;
}

int mytbf_destory(mytbf_t *ptr)
{
    struct mytbf_st *me = ptr;
    job[me->pos] = NULL;
    free(ptr);

    return 0;
}