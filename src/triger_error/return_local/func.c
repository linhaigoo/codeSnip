#include "func.h"

const char *return_local_chr_mod(int mod)
{
    const char local[6] = "local";
    if (mod)
    {
        return "llcal";
    }
    return local;
}

const char *return_local_chr()
{
    const char local[6] = "Local";
    return local;
}