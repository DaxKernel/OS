#ifndef MEM_P_STACK_H
#define MEM_P_STACK_H
    void init_stack(int stack_size);
    void print_stack();
    void push(int mem_addr);
    int pop();
    void free_page(int *page);
#endif