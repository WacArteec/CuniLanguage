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

    FILE* Asmer = fopen("", "r");

    //Translator(tree.root, &names, Asmer);

    return 0;
}