#include <stdbool.h>

// This function must be implemented by a separate test.c file
void D_UNIT_run();

void D_UNIT_ASSERT(bool condition, const char *t_name);

void D_UNIT_ASSERT_FALSE(bool condition, const char *t_name);