#include <kernel/k_datastruct/circular_buffer.h>
#include <stdbool.h>

static circular_buf_t cbuf;
static unsigned char buf[15];

void kbd_buffer_init(){
    cbuf = circular_buf_init(buf, 15);
}

void kbd_insert(unsigned char key){
    circular_buf_put(&cbuf, key);
}

void get_str(char *str)
{
    //reset_buffer();
    unsigned char t = '\0';
    char *ptr = str;
    while (true)
    {
        if (!circular_buf_empty(&cbuf))
        {
            circular_buf_get(&cbuf, &t);
            if (t != '\n')
            {
                *ptr = t;
                ++ptr;
            }
            else
                break;
        }
    }
    *ptr = '\0';
}

void get_int(int *input)
{
    //reset_buffer();
    int t = 0;
    unsigned char c;
    while (true)
    {
        if (!circular_buf_empty(&cbuf))
        {
            circular_buf_get(&cbuf, &c);
            if (c != '\n')
            {
                t = (t * 10) + (c - '0');
            }
            else
                break;
        }
    }
    *input = t;
}
