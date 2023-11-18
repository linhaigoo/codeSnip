#include <stdarg.h>
#include <stdio.h>

float average_void(void)
{
    return 0.0f;
}

float average_empty()
{
    int n;
    va_list args;
    int i = 0;
    float sum = 0;

    // va_start(args, n);
    n = va_arg(args, int);
    n = 4;
    printf("agrn: %d\n", n);

    for (i = 0; i < n; i++)
    {
        int arg = va_arg(args, int);
        printf("agr%d: %d\n", i, arg);
        sum += arg;
    }

    va_end(args);
    return sum / n;
}

int main()
{
    int n = 1;
    int num1 = 1;
    int num2 = 2;
    float f = average_empty(n, num1, num2);
    printf("average %f\n", f);
    f = average_empty(n, num1);
    printf("average %f\n", f);
    // f = average_void(n, num1);
    f = average_void();
    printf("average %f\n", f);
}