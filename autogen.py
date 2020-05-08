"""
Generate `name` template specialization functions from the enum structs in a given
input file. The function returns the name of an enum member based on its value.

Outputs the code in files created by replacing the input file's .def extension by
.cpp and .hpp. 
"""

import os
import sys

def printUsage():
    print("Usage : autogen.py <source root> <input file> <namespace for name function>")
    print("Example : autogen.py C:\\dev\\myDopeProject\\src render\\enums.def render::utils")
    quit()

if len(sys.argv) != 4:
    printUsage()

os.chdir(sys.argv[1])

input = sys.argv[2]
if input.split(".")[-1] != "def":
    print("Tool accepts *.def files.")
    printUsage()

output = ".".join(input.split(".")[0:-1])

prefix = sys.argv[3]

namespace = []

with open(input) as f:
    rawContent = f.readlines()
    content = [ x.strip().split(" ") for x in rawContent ]
    
    enumValues = {}

    i = 0
    while i < len(content):
        if content[i][0] == "namespace":
            namespace.append(content[i][1])
            # skip '{'
            i += 1
        elif content[i][0] == "}": # end of 'namespace'
            namespace.pop()
        elif content[i][0] == "enum" and content[i][1] == "struct":
            enumName = "::".join(namespace) + "::" + content[i][2]
            enumValues[enumName] = []
            # skip '{'
            i += 1
            while content[i][0] != "};":
                if content[i][0].isalnum() and content[i][1] == "=":
                    enumValues[enumName].append(content[i][0])
                i += 1
        i += 1

with open(output + ".hpp", "w") as f:
    f.write("// This file is auto-generated. Do not edit !\n\n")
    f.write("#pragma once\n\n")
    f.writelines(rawContent)
    f.write("\n")

    f.write("namespace " + prefix + """
{
\t/**
\t * Extract the name of a scoped enum value.
\t */
\ttemplate <typename Enum> const char* name(Enum e);
""")
    for enum in enumValues:
        f.write("\ttemplate <> const char* " + "name<" + enum + ">(" + enum + " e);\n")
    f.write("}\n")

with open(output + ".cpp", "w") as f:
    f.write("// This file is auto-generated. Do not edit !\n\n")
    f.write('#include "' + os.path.relpath(output).replace("\\", "/") + '.hpp"\n\n')
    f.write("#include <unordered_map>\n\n")

    for enum in enumValues:
        rawEnum = enum.split("::")[-1]
        f.write("static const std::unordered_map<" + enum + ", const char*> " + rawEnum + "_names = {\n")
        for name in enumValues[enum]:
            f.write('\t{' + enum + '::' + name + ', "' + rawEnum + '::' + name + '"},\n')
        f.write("};\n")

        f.write("template <> const char* " + prefix + "::name<" + enum + ">(" + enum + " e) { return " + rawEnum + "_names.find(e)->second; }\n\n")
