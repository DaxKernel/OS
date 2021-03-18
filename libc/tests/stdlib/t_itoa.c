#include <tests/t_itoa.h>

void D_TEST_itoa_zero(){
    char s[2];
    itoa(0, s);
    D_UNIT_ASSERT(s[0] == '0' && s[1] == '\0', "itoa_on_zero_input");
}