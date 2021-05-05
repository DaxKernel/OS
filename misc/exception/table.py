e_table = dict()


def insert_table(vector, name):
    e_table[vector] = name


def generate_exception_table():
    insert_table(0, "Divide By Zero")
    insert_table(1, "Debug")
    insert_table(2, "Non-Maskable Interrupts")
    insert_table(3, "Breakpoint")
    insert_table(4, "Overflow")
    insert_table(5, "Bound Range Exceeded")
    insert_table(6, "Invalid Opcode")
    insert_table(7, "Device Not Available")
