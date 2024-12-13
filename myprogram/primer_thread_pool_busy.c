#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

/*
* 创建1个线程池，使得每个线程都可以对一个数来计算其是否为素数
* 此版本的缺点：
*
*/

#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

static void *thr_prime(void *p);

int main()
{
    int i, err;
    pthread_t tid[THRNUM];

    // 创建多个线程，形成线程池
    for(i = 0; i <= THRNUM; i++)
    {
        err = pthread_create(tid + i, NULL, thr_prime, (void*)i);
        if (err)
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(err));
        }
    }

    // 分配任务，等待线程抢夺任务
    for (i = LEFT; i <= RIGHT; i++)
    {
        pthread_mutex_lock(&mut_num);
        // 在忙等num的值变为0，应该如何优化呢？其实这个地方应该使用条件变量
        while(num != 0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }
        num = i;
        pthread_mutex_unlock(&mut_num);
    }

    // 任务执行完成之后，通知所有线程任务执行完毕
    pthread_mutex_lock(&mut_num);
    while (num != 0)
    {
        pthread_mutex_unlock(&mut_num);
        sched_yield();
        pthread_mutex_lock(&mut_num);
    }
    num = -1;
    pthread_mutex_unlock(&mut_num);

    // 回收所有线程
    for (i = 0; i < THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mut_num);
    exit(0);
}

static void *thr_prime(void *p)
{
    int i, j, mark;
    while (1)
    {
        pthread_mutex_lock(&mut_num);
        // // 在忙等num的值变为非0，应该如何优化呢？其实这个地方应该使用条件变量
        while (num == 0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }
        if (num == -1)
        {
            pthread_mutex_unlock(&mut_num);
            // 注意临界区的跳转语句，防止死锁
            break;
        }
        i = num;
        num = 0;
        pthread_mutex_unlock(&mut_num);

        mark = 1;
        for (j = 2; j < i / 2; j++)
        {
            if (i % j == 0)
            {
                mark = 0;
                break;
            }
        }
        if (mark)
        {
            printf("[tid %d]%d is a primer\n", (int)p, i);
        }
    }
}