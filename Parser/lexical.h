#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../tree/tree.h"
#include "../MyAssert/myassert.h"

union LexData
{
    double value;

    int oper;
    int sort;

    unsigned int var;
};

struct Token
{
    Type type;

    LexData data = {};

    unsigned int line;
    unsigned int symbol;
};

struct VarTable
{
    char* name = NULL;
    unsigned int order = 0;
    unsigned int scope = 0;
};

struct NameTable
{
    struct VarTable* vars = NULL;
    size_t size = 0;
};

enum Sizes
{
    DEF_SIZE = 16
};

Token* LexicalAnalis(char* text, struct NameTable* names);

#endif //LEXICAL_H