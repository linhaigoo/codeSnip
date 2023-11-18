#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *a = "066676777";
    int state = 0;
    int i = 0;
    while (state != 6 && i < 9)
    {
        switch (state)
        {
        case 0:
            if (a[i] == '0')
                state = 1;
            else if (a[i] == 'O')
                state = 2;
            else if (a[i] >= '0' && a[i] <= '9')
                state = 3;
            else
            {
                state = 6;
                i = 9;
            }
            break;
        case 1:
            if (a[i] == 'x')
            {
                state = 4;
                i = 9;
            }
            else if (a[i] >= 48 && a[i] <= 55)
            {
                state = 5;
                while (i < 9)
                    if (a[i] >= 48 && a[i] <= 55)
                        ++i;
                    else
                    {
                        state = 6;
                        i = 9;
                    }
            }
            else
            {
                state = 6;
                i = 9;
            }
            break;
        case 2:
            if (a[i] == 'X')
            {
                state = 4;
                i = 9;
            }
            else
            {
                state = 6;
                i = 9;
            }
            break;
        case 3:
            while (i < 9)
                if (a[i] >= 48 && a[i] <= 57)
                    ++i;
                else
                {
                    state = 6;
                    i = 9;
                }
            break;
        default:
            printf("please select correct initial state");
            break;
        }
        ++i;
    }
    if (state == 3)
        printf("it is a decimal number");
    else if (state == 4)
        printf("it is a hexadecimal number");
    else if (state == 5)
        printf("it is a octal number");
    else
        printf("error encountered as invalid string");
}