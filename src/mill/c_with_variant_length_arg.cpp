#include <stdio.h>
/*要使用变长参数的宏，需要包含下面的头文件*/
#include <stdarg.h>
/*
 * getSum：用于计算一组整数的和
 * num：整数的数量
 *
 * */
int getSum(int num,...)
{
    va_list ap;//定义参数列表变量
    int sum = 0;
    int loop = 0;
    int value = 0;
    va_start(ap,num);
    for(;loop < num; loop++)
    {
        value = va_arg(ap,int);
        printf("the %d value is %d\n",loop,value);
        sum += value;
    }
    va_end(ap);
    return sum;
}
int main(int argc,char *argv[])
{
    int sum = 0;
    float a =  8.25f;
    printf("a to int=0X%jx\n",*(int*)&a);
    sum = getSum(5,a,2,3,4,5);//-m32
    printf("%d\n",sum);
    return 0;
}
