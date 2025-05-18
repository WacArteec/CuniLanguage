#ifndef TREE_H
#define TREE_H

#define $$$ 

#include <stdio.h>
#include <stdlib.h>

#include "../MyAssert/myassert.h"

enum Side
{
    ZERO,
    LEFT,
    RIGHT
};

enum Type
{
    DEF_TYPE,
    NUM,
    OPER,
    VAR,
    SORT
};

enum Operations
{
    DEF_OPER,
    OP_BR,
    CL_BR,
    ADD,
    SUB,
    MULT,
    DIV,
    POW,
    LOG,
    SIN,
    COS,
    TAN,
    COMMA,
    INIT,
    IF,
    ELSE,
    TILL,
    OP_BODY,
    CL_BODY,
    EQUAL,
    NOTEQUAL,
    ABOVE,
    BELOW,
    EQABOVE,
    EQBELOW,
    STR_END,
    FUNC,
    ARGS_FUNC,
    END
};

enum Sort
{
    VOID,
    IS_FUNC,
    INT
};

struct Tree;

union Data
{
    double value;
    int oper;
    int sort;
    unsigned int var;
};

struct Node
{
    Type type = DEF_TYPE;

    Data data = {};

    Node* left = NULL;
    Node* right = NULL;
};

struct Tree
{
    Node* root = NULL;
    unsigned int size = 0;
};

Node* CreateFict(struct Node* left, struct Node* right);
Node* CreateNum (float value);
Node* CreateVar (struct Node* left, struct Node* right, unsigned int var);
Node* CreateOper(struct Node* left, struct Node* right, Operations oper);
Node* CreateSort(                   struct Node* right, Sort sort);
Node* DeleteNode(struct Node* node);
void SaveTree(struct Tree* tree);
void PrintNode(FILE* stream, struct Node* leaf);
void Ctor(struct Tree* tree);
void Dtor(struct Tree* tree);
void GraphDump(struct Tree* tree);

#endif //TREE_H