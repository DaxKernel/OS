#include <kernel/display/common.h>
/**
 * Pointer to start of current line
 */
void *ptr_to_y_pos()
{
    void *result = (char *)vbe_info.framebuffer + vbe_info.pitch * position.y;
    return result;
}

uint32_t skip_line(const int n)
{
    return vbe_info.pitch * n;
}
