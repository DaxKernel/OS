# Code generator for CPU Exceptions
# Generate interrupt code that kernel panics with an indication of the exception


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


include_header = "#include <kernel/interrupt/exceptions.h>\n#include <kernel/panic.h>\n#include <kernel/interrupt/pic.h>\n\n"
include_irq = "#include <kernel/interrupt/irq_handlers.h>\n\n"
warning_comment = "/**\nWARNING: This is an auto-generated file\n**/\n\n"


def get_definition_text(vector, text):
    t = f"IRQ_HANDLER cpu_exception_{vector}(IRQ_ARG)\n{{\n\tsend_eoi();\n\tk_panic(\"{text}\");\n}}\n\n"
    return t


def get_declaration_text(vector, text):
    t = f"// {text}\nIRQ_HANDLER cpu_exception_{vector}(IRQ_ARG);\n\n"
    return t


def create_writer_function(dir, getter, include):
    def writer():
        try:
            f = open(dir, "w+")
            f.write(warning_comment)
            f.write(include)
            for k, v in e_table.items():
                f.write(getter(k, v))
        except Exception as e:
            print("Error:", e)
        else:
            f.close()
    return writer


gen_c_file = create_writer_function(
    "../../kernel/arch/i386/interrupts/exceptions.c", get_definition_text, include_header)

gen_h_file = create_writer_function(
    "../../kernel/include/kernel/interrupt/exceptions.h", get_declaration_text, include_irq)


def generate_calls():
    for k, v in e_table.items():
        print(f"add_interrupt({k}, (uint32_t)cpu_exception_{k});")


if __name__ == "__main__":
    generate_exception_table()
    gen_c_file()
    gen_h_file()
    generate_calls()
