/*
Implementation of circular buffer from embeddedartistry.com
Modified to suit current requirements:
Removed opaque type and dynamic allocation
*/

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// No malloc, No opaque types
typedef struct circular_buf_t {
    uint8_t *buffer;
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
}circular_buf_t;

typedef circular_buf_t* cbuf_handle_t;

circular_buf_t circular_buf_init(uint8_t* buffer, size_t size);

void circular_buf_reset(cbuf_handle_t cbuf);

void circular_buf_put(cbuf_handle_t cbuf, uint8_t data);

int circular_buf_get(cbuf_handle_t cbuf, uint8_t * data);

bool circular_buf_empty(cbuf_handle_t cbuf);

bool circular_buf_full(cbuf_handle_t cbuf);

size_t circular_buf_capacity(cbuf_handle_t cbuf);

size_t circular_buf_size(cbuf_handle_t cbuf);

#endif
