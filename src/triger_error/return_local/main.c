#include "func.h"
#include <string.h>

const char *local_return_local_chr()
{
    const char local[6] = "nocal";
    return local;
}

int main()
{
    char buff[60];
    const char *str0 = return_local_chr(1);
    printf("func ret %s\n", str0);
    const char *str1 = return_local_chr_mod(0);
    printf("func ret %s\n", str1);
    const char *str3 = local_return_local_chr(1);
    printf("func ret %s\n", str3);
    strncpy(buff, str0, 60);
}