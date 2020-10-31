#include <stdio.h>

#include <kernel/tty.h>

void print_header(){
    terminal_initialize();
    terminal_write_string_centered("DAX Operating System");
    terminal_write_string_centered("Build 1.0 2020 (c)");
    terminal_print_horizontal_rule('-');
}

void kernel_main(void) {
    print_header();
}
