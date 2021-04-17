#include <kernel/ds/circular_buffer.h>

static void advance_pointer(cbuf_handle_t cbuf)
{
    if (circular_buf_full(cbuf))
    {
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    }

    cbuf->head = (cbuf->head + 1) % cbuf->max;

    // We mark full because we will advance tail on the next time around
    cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(cbuf_handle_t cbuf)
{
    cbuf->full = false;
    cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}

circular_buf_t circular_buf_init(uint8_t *buffer, size_t size)
{
    circular_buf_t cbuf;
    cbuf.buffer = buffer;
    cbuf.max = size;
    circular_buf_reset(&cbuf);
    return cbuf;
}

void circular_buf_reset(cbuf_handle_t cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
}

size_t circular_buf_size(cbuf_handle_t cbuf)
{
    size_t size = cbuf->max;
    if (!circular_buf_full(cbuf))
    {
        if (cbuf->head >= cbuf->tail)
        {
            size = (cbuf->head - cbuf->tail);
        }
        else
        {
            size = (cbuf->max + cbuf->head - cbuf->tail);
        }
    }
    return size;
}

size_t circular_buf_capacity(cbuf_handle_t cbuf)
{
    return cbuf->max;
}

void circular_buf_put(cbuf_handle_t cbuf, uint8_t data)
{
    cbuf->buffer[cbuf->head] = data;
    advance_pointer(cbuf);
}

int circular_buf_get(cbuf_handle_t cbuf, uint8_t *data)
{
    int r = -1;

    if (!circular_buf_empty(cbuf))
    {
        *data = cbuf->buffer[cbuf->tail];
        retreat_pointer(cbuf);

        r = 0;
    }

    return r;
}

bool circular_buf_empty(cbuf_handle_t cbuf)
{
    return (!circular_buf_full(cbuf) && (cbuf->head == cbuf->tail));
}

bool circular_buf_full(cbuf_handle_t cbuf)
{
    return cbuf->full;
}
