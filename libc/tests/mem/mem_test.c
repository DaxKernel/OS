#include <tests/mem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void D_TEST_dynamic_array_10()
{
    int *p = malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i)
        p[i] = i;
    D_UNIT_ASSERT(p != NULL, "MEM/Dynamic_array_10");
    free(p);
}

void D_TEST_unfulfillable_request()
{
    char *p = malloc(/* Quick way to get SIZE_MAX*/ (size_t)-1);
    D_UNIT_ASSERT_FALSE(p, "MEM/UNFULFILLABLE_MEM_REQUEST");
}

void D_TEST_malloc_span_multiple_pages()
{
    char *p = malloc(4096 * 5);
    char *q = malloc(1);
    D_UNIT_ASSERT(q > p + 4096, "MEM/REQUEST_SPAN_OVER_MULTIPLE_PAGES");
    free(p);
    free(q);
}