/************a.c**************/
#define TEST
#include "include_test.h"

struct test_st gen_struct()
{
    struct test_st st;
    st.el0 = 1;
    st.el1 = 2;
    st.el2 = 3;
    return st;
}