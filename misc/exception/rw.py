from table import *
import re
import os


def create_writer_function(dir, getter, include, warning):
    def writer():
        try:
            f = open(dir, "w+")
            f.write(warning)
            f.write(include)
            for k, v in e_table.items():
                f.write(getter(k, v))
        except Exception as e:
            print("Error:", e)
        else:
            f.close()
    return writer


def read_from_file(path, new_line=True, comment=False):
    try:
        f = open(path, "r")
        text = f.read()
        if(comment):
            text = "/**\n" + text + "\n**/"
        if(new_line):
            text += "\n\n"
        return text
    except Exception as e:
        print("Error: ", e)
    else:
        f.close()


def getMatch(pattern, string):
    regex = re.compile(pattern, re.M)
    match = regex.search(string)
    if match == None:
        raise SyntaxError("paths.config contains syntax errors!")
    else:
        return os.path.expanduser(match[1])


def get_path_from_config():
    try:
        f = open("./config/paths.config", "r")
        content = f.read()
        path_c = getMatch("^path_c = (.+\.c)", content)
        path_h = getMatch("^path_h = (.+\.h)", content)
        return (path_c, path_h)
    except Exception as e:
        print("Error: ", e)
    else:
        f.close()
