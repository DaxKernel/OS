#include <kernel/mem/p_stack.h>
#include <stdio.h>

static uint32_t *stack_start;
static uint32_t *stack_end;
static uint32_t *top;

extern char _kernel_end;

void init_stack(int stack_size){
    stack_start = (uint32_t *)&_kernel_end;
    printf("Stack starts at %d\n", stack_start);
    stack_end = stack_start + stack_size;
    printf("Stack ends at %d\n\n", stack_end);
    top = stack_start - 1;
}

void push(uint32_t mem_addr){
    if(top < stack_end){\
        top = top + 1;
        *top = mem_addr;
    }
    else{
        // Stack Overflow
        // Kernel Panic here
    }
}

uint32_t pop(){
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
    uint32_t *p = stack_start;
    printf("stack[start] = %d\n", *p);
    p = stack_end - 1;
    printf("stack[end] = %d\n", *p);
    printf("top = %d\n\n", *top);
}