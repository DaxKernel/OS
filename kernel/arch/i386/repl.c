#include <kernel/repl.h>
#include <kernel/display/tty.h>
#include <kernel/display/draw.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <kernel/io.h>
#include <kernel/panic.h>
void repl(){
    char command[20];
    tty_write_string("$root:");
    scanf("%s", command);
    if(strcmp(command, "nyancat") == 0)
        draw_image();
    else if(strcmp(command, "colortext") == 0){
        tty_print_rainbow("Hello from the world of DaxOS!");
    }
    else if(strcmp(command, "shutdown") == 0){
        outportb(0x604, 0x2000);
    }
    else if(strcmp(command, "panic") == 0){
        k_panic("This is a demo of a panic() in DaxOS.");
    }
    tty_insert_char('\n');
}

void accept_commands(){
    while(true){
        repl();
        asm("hlt");
    }
}