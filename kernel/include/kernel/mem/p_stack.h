#ifndef MEM_P_STACK_H
#define MEM_P_STACK_H
    #include <stdint.h>

    void init_stack(int stack_size);
    void print_stack();
    void push(uint32_t mem_addr);
    int32_t pop();
    int get_stack_count();
#endif