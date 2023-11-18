/************main.c**************/
#include "include_test.h"
#include <stdio.h>
struct test_st gen_struct();
int main()
{
    struct test_st st = gen_struct();
    printf("st.el1 %d, st.el2 %f\n", st.el1, st.el2);
}