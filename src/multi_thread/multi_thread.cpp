/*
 * @Author: your name
 * @Date: 2021-08-02 20:39:46
 * @LastEditTime: 2021-08-07 13:28:53
 * @LastEditors: Kian Liu
 * @Description: In User Settings Edit
 * @FilePath: /codeSnip/src/multi_thread.cpp
 */
#include "multi_thread.h"
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;
pthread_t thread1, thread2;

bool bind_cpu_core(uint32_t cpu_id)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);
    return (sched_setaffinity(0, sizeof(mask), &mask) != -1);
}

void pid_cycle()
{
    // pthread_mutex_lock(&mutex1);
    counter++;
    // printf("Counter value: %d\n", counter);
    // printf("getpid: %d getpthread_self: %lu tid:%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));
    // pthread_mutex_unlock(&mutex1);
}

void *loop_func(void *arg)
{
    uint32_t core_id = *(uint32_t *)(arg);
    printf("core_id %d\n", core_id);
    // bind_cpu_core(core_id);

    for (;;)
    {
        pid_cycle();
    }
}

int start(int code_id)
{
    bind_cpu_core(code_id);
    uint32_t core1_id = 3, core2_id = 4;
    pthread_create(&thread1, NULL, &loop_func, &core1_id);
    pthread_create(&thread2, NULL, &loop_func, &core2_id);
    sleep(1);
    return 1;
}

int stop(int arg)
{
    int kill1ret = pthread_kill(thread1, SIGINT);
    int kill2ret = pthread_kill(thread1, SIGINT);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("kill1ret %d kill2ret %d \n", kill1ret, kill2ret);
    return 0;
}
