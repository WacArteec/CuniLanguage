#include <stdio.h>

#include "reader/reader.h"
#include "tree/tree.h"
#include "Parser/lexical.h"
#include "Parser/syntactic.h"
#include "TranslatorNasm/translatorNasm.h"


int main()
{
    Tree tree = {};
    Ctor(&tree);

    NameTable names = {};

    char* program_text = Input("example.cl");

    tree.root = Parser(program_text, &tree, &names);

    FILE* Asmer = fopen("program.asm", "w");
    MyAssert(Asmer, FILES);

    TranslatorNasm(tree.root, &names, Asmer);
    fclose(Asmer);

    NameTableDtor(&names);

    system("nasm -f elf64 program.asm -o program.o && ld program.o -o program");


    return 0;
}