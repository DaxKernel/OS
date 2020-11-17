#include <tests/t_strlen.h>
#include <d_unit/d_unit.h>
#include <string.h>

void D_TEST_strlen_on_normal_string()
{
    const char *str = "Hello World";
    const int l = 11;
    const int c_len = strlen(str);
    D_UNIT_ASSERT(l == c_len, "strlen_on_normal_string");
}

void D_TEST_strlen_on_empty_string()
{
    const char *str = "";
    const int c_len = strlen(str);
    D_UNIT_ASSERT_FALSE(c_len, "strlen_on_empty_string");
}