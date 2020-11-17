#include "d_unit.h"
#include <stdio.h>

void D_UNIT_ASSERT(bool condition, const char *t_name)
{
    if (!condition)
    {
        printf("Test Failed: ");
        printf(t_name);
        printf("\n");
    }
}

void D_UNIT_ASSERT_FALSE(bool condition, const char *t_name)
{
    D_UNIT_ASSERT(!condition, t_name);
}