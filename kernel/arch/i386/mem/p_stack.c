#include <kernel/mem/p_stack.h>
#include <stdio.h>

int *stack_start;
int *stack_end;
int *top;
int push_count = 0;

extern char _kernel_end;

void init_stack(int stack_size){
    stack_start = (int *)&_kernel_end;
    printf("Stack starts at %d\n", stack_start);
    stack_end = stack_start + stack_size;
    printf("Stack ends at %d\n\n", stack_end);
    top = stack_start - 1;
}

void push(int mem_addr){
    if(top < stack_end){\
        top = top + 1;
        *top = mem_addr;
        push_count++;
    }
    else{
        // Stack Overflow
        // Kernel Panic here
    }
}

int pop(){
    if(top >= stack_start){
        return *top--;
    }
    else{
        // Stack Underflow
        // Kernel Panic here; Out Of Memory
        return -1;
    }
}

void print_stack(){
    int *p = stack_start;
    printf("stack[start] = %d\n", *p);
    p = stack_end - 1;
    printf("stack[end] = %d\n", *p);
    printf("top = %d\n\n", *top);
    /*printf("Total pushes = %d\n", push_count); */
}