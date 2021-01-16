#include <kernel/c_buffer.h>
#include <stdio.h>

char buffer[16];

char *d_begin = buffer;
char *d_end = buffer + 16;
char *d_insert = buffer;
char *d_read = buffer;

int write_count = 0;

struct
{
    bool EMPTY;
    bool FULL;
} BUFFER_STATUS;

bool is_buffer_full()
{
    return BUFFER_STATUS.FULL;
}

bool is_buffer_empty()
{
    return BUFFER_STATUS.EMPTY;
}

void set_status()
{
    if (write_count > 0)
        BUFFER_STATUS.EMPTY = false;
    else
        BUFFER_STATUS.EMPTY = true;

    if (write_count >= 15)
        BUFFER_STATUS.FULL = true;
    else
        BUFFER_STATUS.FULL = false;
}
void insert_buffer(char data)
{
    *d_insert = data;
    d_insert = (d_insert + 1 < d_end) ? d_insert + 1 : d_begin;
    write_count = write_count + 1;
    set_status();
}

char read_buffer()
{
    char ret = *d_read;
    d_read = (d_read + 1 > d_begin) ? d_read + 1 : d_begin;
    write_count = write_count - 1;
    set_status();
    return ret;
}

void reset_buffer()
{
    d_begin = buffer;
    d_end = buffer + 16;
    d_insert = buffer;
    d_read = buffer;
    BUFFER_STATUS.EMPTY = true;
    BUFFER_STATUS.FULL = false;
}