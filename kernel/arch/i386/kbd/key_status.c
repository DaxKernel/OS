#include <kernel/kbd/key_status.h>

KEY_STATUS_T KEY_STATUS;

void set_key_status(bool CAPS_LOCK, bool NUM_LOCK, bool SCROLL_LOCK)
{
    KEY_STATUS.CAPS_LOCK = CAPS_LOCK;
    KEY_STATUS.NUM_LOCK = NUM_LOCK;
    KEY_STATUS.SCROLL_LOCK = SCROLL_LOCK;
}