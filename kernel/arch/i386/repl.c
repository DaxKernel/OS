#include <kernel/repl.h>
#include <kernel/display/tty.h>
#include <kernel/display/draw.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void repl(){
    char command[20];
    tty_write_string("$root:");
    scanf("%s", command);
    if(strcmp(command, "nyancat") == 0)
        draw_image();
    else if(strcmp(command, "colortext") == 0){
        tty_print_rainbow("Hello from the world of DaxOS!");
    }
    tty_insert_char('\n');
}

void accept_commands(){
    while(true){
        repl();
        asm("hlt");
    }
}