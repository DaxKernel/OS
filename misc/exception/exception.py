# Code generator for CPU Exceptions
# Generate interrupt code that kernel panics with an indication of the exception
from rw import *
from table import *


include_header = read_from_file("./config/exception_c_i.c")
include_irq = read_from_file("./config/exception_h_i.c")
warning_comment = read_from_file("./config/warning.txt", True, True)


def get_definition_text(vector, text):
    t = f"IRQ_HANDLER cpu_exception_{vector}(IRQ_ARG)\n{{\n\tsend_eoi();\n\tk_panic(\"{text}\");\n}}\n\n"
    return t


def get_declaration_text(vector, text):
    t = f"// {text}\nIRQ_HANDLER cpu_exception_{vector}(IRQ_ARG);\n\n"
    return t


(path_c, path_h) = get_path_from_config()

gen_c_file = create_writer_function(
    path_c, get_definition_text, include_header, warning_comment)

gen_h_file = create_writer_function(
    path_h, get_declaration_text, include_irq, warning_comment)


def generate_calls():
    for k, v in e_table.items():
        print(f"add_interrupt({k}, (uint32_t)cpu_exception_{k});")


if __name__ == "__main__":
    generate_exception_table()
    gen_c_file()
    gen_h_file()
    generate_calls()
    get_path_from_config()
