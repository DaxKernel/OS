#include <kernel/repl.h>
#include <kernel/display/tty.h>
#include <kernel/display/draw.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <kernel/io.h>
#include <kernel/panic.h>
void repl()
{
    char command[20];
    tty_write_string("$root:");
    scanf("%s", command);
    if (strcmp(command, "nyancat") == 0)
        draw_image();
    else if (strcmp(command, "colortext") == 0)
    {
        tty_print_rainbow("Hello from the world of DaxOS!");
    }
    else if (strcmp(command, "shutdown") == 0)
    {
        outportb(0x604, 0x2000);
    }
    else if (strcmp(command, "panic") == 0)
    {
        k_panic("This is a demo of a panic() in DaxOS.");
    }
    else if (strcmp(command, "repeat") == 0)
    {
        char rep[100];
        printf("What do you want to repeat?\n");
        scanf("%s", &rep);
        printf("You wrote: %s", rep);
    }
    else if (strcmp(command, "mem") == 0)
    {
        int size;
        printf("Enter size of array:");
        scanf("%d", &size);
        if (size <= 0)
        {
            printf("Invalid size!\n");
        }
        else
        {
            int *array = malloc(sizeof(int) * size);
            for (int i = 0; i < size; ++i)
            {
                printf("Enter element %d: ", i + 1);
                scanf("%d", &array[i]);
            }
            printf("You entered:\n");
            for (int i = 0; i < size; ++i)
            {
                printf("%d\n", array[i]);
            }
            free(array);
        }
    }
    else
    {
        printf("%s command not found!!", command);
    }
    tty_insert_char('\n');
    tty_insert_char('\n');
}

void accept_commands()
{
    while (true)
    {
        repl();
        asm("hlt");
    }
}