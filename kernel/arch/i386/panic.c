#include <kernel/panic.h>
#include <kernel/display/tty.h>

static inline void block_forever()
{
    asm("cli");
    asm("hlt");
}

void show_title()
{
    tty_insert_char('\n');
    tty_write_string_centered(" A JOB FOR   |   KERNEL   |   DAX\n");
    tty_write_string_centered("Rubb3rDuck  |   PANIC    |   OS\n");
    tty_insert_char('\n');
    tty_write_string_centered(" AN UNRECOVERABLE ERROR OCCURRED\n\n");
    tty_print_seperator();
    tty_insert_char('\n');
}

void k_panic(const char *message)
{
    tty_clear();
    show_title();
    tty_write_string_centered(message);
    block_forever();
}
