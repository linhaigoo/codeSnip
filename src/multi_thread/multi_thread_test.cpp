/*
 * @Author: Kian Liu
 * @Date: 2021-08-02 21:40:59
 * @LastEditTime: 2021-08-07 22:06:35
 * @LastEditors: Kian Liu
 * @Description:
 * @FilePath: /codeSnip/src/multi_thread/multi_thread_test.cpp
 */
#include "multi_thread.h"
#include <stdio.h>
#include <unistd.h>
int reuse_stack()
{
    char str[] = {"12345\n"};
    int a = 0;
    int b = 3;
    printf("a+b %d\n", a + b);
    printf("%s", str);
    return 0;
}

int multi_thread_test()
{
    start(2);
    reuse_stack();
    sleep(10);
    stop(1);
    return 0;
}

int main()
{
    multi_thread_test();
    return 0;
}
