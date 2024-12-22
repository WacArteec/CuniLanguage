#include <assert.h>
#include <ctype.h>

#include "../tree/tree.h"
#include "lexical.h"
#include "syntactic.h"

Node* GetStrEnd (struct Token* tokens);
Node* GetFunc   (struct Token* tokens);
Node* GetIfTill  (struct Token* tokens);
Node* GetElse   (struct Token* tokens);
Node* GetEqual  (struct Token* tokens);
Node* GetInit   (struct Token* tokens);
Node* GetAddSub (struct Token* tokens);
Node* GetMulDiv (struct Token* tokens);
Node* GetTrig   (struct Token* tokens);
Node* GetLog    (struct Token* tokens);
Node* GetDeg    (struct Token* tokens);
Node* GetBracket(struct Token* tokens);
Node* GetNumVar (struct Token* tokens);
Node* GetBody   (struct Token* tokens);
void SyntaxErr(struct Token* tokens);
void CountParam(Node* node, unsigned int* count_param);

unsigned int POS = 0;

Node* Parser(char* text, struct Tree* tree, struct NameTable* names)
{
    Token* tokens = LexicalAnalis(text, names);
    $$$ printf("lexical succed\n");

    Node* node = GetStrEnd(tokens);

    if(tokens[POS].data.oper != END)
        SyntaxErr(tokens);

    free(tokens);

    return node;
}

Node* GetStrEnd(struct Token* tokens) //самая главная, определяет строку
{
    Node* new_node = GetFunc(tokens);

    while (tokens[POS].type == OPER && tokens[POS].data.oper == STR_END)
    {
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetFunc(tokens);

        new_node = CreateOper(nodeleft, noderight, STR_END);
    }
    
    return new_node;
}

Node* GetFunc(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = NULL;

    if(tokens[POS].type == OPER && tokens[POS].data.sort == FUNC)
    {
        new_node = GetNumVar(tokens);

        int op = tokens[POS].data.oper;
        POS += 1;

        Node* nodeleft  = new_node;
        Node* noderight = GetBody(tokens);

        unsigned int count_param = 0;
        CountParam(nodeleft->right, &count_param);
        
        nodeleft->left = CreateNum(count_param);

        new_node = CreateOper(nodeleft, noderight, FUNC);
    }

    else
        new_node = GetIfTill(tokens);

    return new_node;
}

void CountParam(Node* node, unsigned int* count_param)
{
    if(node != NULL)
    {
        *count_param += 1;
        CountParam(node->right, count_param);
    }
}

Node* GetIfTill(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = NULL;

    if(tokens[POS].type == OPER && (  tokens[POS].data.oper == IF 
                                   || tokens[POS].data.oper == TILL))
    {
        int op = tokens[POS].data.oper;
        POS += 1;

        Node* nodeleft = CreateFict(GetEqual(tokens), GetBody(tokens));
        Node* noderight = GetElse(tokens);

        if(op == IF)
        {
            Node* nodeleft = CreateFict(new_node, GetBody(tokens));
            Node* noderight = GetElse(tokens);

            new_node = CreateOper(nodeleft, noderight, IF);
        }

        else
        {
            Node* nodeleft = new_node;
            Node* noderight = GetBody(tokens);

            new_node = CreateOper(nodeleft, noderight, TILL);
        }
    }

    else
        new_node = GetEqual(tokens);

    return new_node;
}

Node* GetElse(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = NULL;

    if(tokens[POS].type == OPER && tokens[POS].data.oper == ELSE)
    {
        POS += 1;

        Node* nodeleft  = NULL;
        Node* noderight = NULL;

        if(tokens[POS].type == OPER && tokens[POS].data.oper == IF)
            nodeleft = GetIfTill(tokens);

        else
            noderight = GetBody(tokens);

        new_node = CreateOper(nodeleft, noderight, IF);
    }

    else
        new_node = GetBody(tokens);

    return new_node;
}

Node* GetEqual(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetInit(tokens);

    if(tokens[POS].type == OPER && (  tokens[POS].data.oper == EQUAL 
                                   || tokens[POS].data.oper == ABOVE 
                                   || tokens[POS].data.oper == EQABOVE 
                                   || tokens[POS].data.oper == BELOW 
                                   || tokens[POS].data.oper == EQBELOW 
                                   || tokens[POS].data.oper == NOTEQUAL))
    {
        Operations op = (Operations) (tokens[POS].data.oper);
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetInit(tokens);

        new_node = CreateOper(nodeleft, noderight, op);
    }

    return new_node;
}

Node* GetInit(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetNumVar(tokens);

    if(tokens[POS].type == OPER && tokens[POS].data.oper == INIT)
    {
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetAddSub(tokens);

        new_node = CreateOper(nodeleft, noderight, INIT);
    }

    return new_node;
}

Node* GetAddSub(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetMulDiv(tokens);

    while(tokens[POS].type == OPER && (  tokens[POS].data.oper == ADD 
                                      || tokens[POS].data.oper == SUB))
    {
        int op = tokens[POS].data.oper;
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetMulDiv(tokens);

        if(op == ADD)
            new_node = CreateOper(nodeleft, noderight, ADD);

        else
            new_node = CreateOper(nodeleft, noderight, SUB);
    }

    return new_node;
}

Node* GetMulDiv(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetTrig(tokens);

    while(tokens[POS].type == OPER && (  tokens[POS].data.oper == MULT
                                      || tokens[POS].data.oper == DIV))
    {
        int op = tokens[POS].data.oper;
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetTrig(tokens);
        
        if(op == MULT)
            new_node = CreateOper(nodeleft, noderight, MULT);

        else
            new_node = CreateOper(nodeleft, noderight, DIV);
    }
    
    return new_node;
}

Node* GetTrig(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = NULL;

    if( tokens[POS].type == OPER && (  tokens[POS].data.oper == SIN
                                    || tokens[POS].data.oper == COS 
                                    || tokens[POS].data.oper == TAN))
    {
        while(tokens[POS].data.oper == SIN || tokens[POS].data.oper == COS || tokens[POS].data.oper == TAN)
        {
            int op = tokens[POS].data.oper;
            POS += 1;

            Node* node = GetLog(tokens);

            if(op == SIN)
                new_node = CreateOper(NULL, node, SIN);

            else if(op == COS)
                new_node = CreateOper(NULL, node, COS);

            else
                new_node = CreateOper(NULL, node, TAN);
        }
    }

    else
        new_node = GetLog(tokens);

    return new_node;
}

Node* GetLog(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetDeg(tokens);

    while(tokens[POS].type == OPER && tokens[POS].data.oper == LOG)
    {
        POS += 1;

        Node* node_left = new_node;
        Node* noderight = GetDeg(tokens);

        return CreateOper(node_left, noderight, LOG);
    }
    return new_node;
}

Node* GetDeg(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetBracket(tokens);

    while(tokens[POS].type == OPER && tokens[POS].data.oper == POW)
    {
        POS += 1;

        Node* node_left = new_node;
        Node* noderight = GetBracket(tokens);

        return CreateOper(node_left, noderight, POW);
    }
    return new_node;
}

Node* GetBracket(struct Token* tokens)
{
$$$ assert(tokens);

    if(tokens[POS].type == OPER && tokens[POS].data.oper == OP_BR)
    {
        POS += 1;

        Node* node = GetAddSub(tokens);

        if(tokens[POS].data.oper != CL_BR)
            SyntaxErr(tokens);
    
        POS += 1;
        return node;
    }

    else
        return GetNumVar(tokens);
}

Node* GetNumVar(struct Token* tokens)
{
$$$ assert(tokens);

    if(tokens[POS].type == VAR)
    {
        POS += 1;

        Node* node = CreateVar(NULL, NULL, tokens[POS - 1].data.var);

        if(tokens[POS].type == OPER && tokens[POS].data.oper == COMMA)
        {
            POS += 1;
            node->right = GetNumVar(tokens);
        }
        
        if(tokens[POS].type == OPER && tokens[POS].data.oper == OP_BR)
        {
            node->right = GetBracket(tokens);
        }

        return node;
    }

    if(tokens[POS].type == NUM)
    {
        POS += 1;
        
        Node* node = CreateNum(tokens[POS - 1].data.value);
        return node;
    }

    else
        return NULL;
}

Node* GetBody(struct Token* tokens)// самая неважная, юзается только после if, till, func
{
$$$ assert(tokens);

    if(tokens[POS].type == OPER && tokens[POS].data.oper == OP_BODY)
    {
        POS += 1;

        Node* node = GetStrEnd(tokens);

        if(tokens[POS].data.oper != CL_BODY)
            SyntaxErr(tokens);
    
        POS += 1;
        return node;
    }

    else
        return NULL;
}

void SyntaxErr(struct Token* tokens)
{
$$$ assert(tokens);
    printf("error on: line = %u symbol = %u\n", tokens[POS].line, tokens[POS].symbol);

    exit(1);
}