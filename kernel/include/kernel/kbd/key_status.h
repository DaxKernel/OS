#ifndef KBD_STATUS_H
#define KBD_STATUS_H

#include <stdbool.h>

typedef struct
{
    bool CAPS_LOCK;
    bool NUM_LOCK;
    bool SCROLL_LOCK;
} KEY_STATUS_T;

extern KEY_STATUS_T KEY_STATUS;

void set_key_status(bool CAPS_LOCK, bool NUM_LOCK, bool SCROLL_LOCK);

#endif