#include "../tree/tree.h"
#include "lexical.h"
#include "syntactic.h"

#define MIN_PRIORITY 0

Node *ROOT_NODE = NULL;

Node *GetStrEnd(struct Token *tokens, size_t *POS);
Node *GetFunc(struct Token *tokens, size_t *POS);
Node *GetIfTill(struct Token *tokens, size_t *POS);
Node *GetElse(struct Token *tokens, size_t *POS);
Node *GetEqual(struct Token *tokens, size_t *POS);
Node *GetInit(struct Token *tokens, size_t *POS);
Node *GetNumVar(struct Token *tokens, size_t *POS);
Node *GetBody(struct Token *tokens, size_t *POS);
void SyntaxErr(struct Token *tokens, size_t *POS);
void CountParam(Node *node, unsigned int *count_param);
Node *GetExpr(Token *tokens, size_t *POS, int min_priority);
Node *GetPrimary(Token *tokens, size_t *POS);

void CountParam(Node *node, unsigned int *count_param)
{
    $$$ printf("CountParam\n");
    $$$ if (node != NULL)
    {
        *count_param += 1;
        CountParam(node->right, count_param);
    }
}

int GetPriority(Operations oper)
{
    switch (oper)
    {
        case ADD:
        case SUB:
            return 1;

        case MULT:
        case DIV:
            return 2;

        case POW:
            return 3;

        default:
            return 0;
    }
}

Node *Parser(char *text, struct Tree *tree, struct NameTable *names)
{
    Token *tokens = LexicalAnalis(text, names);
    $$$ printf("lexical succed\n");

    size_t POS = 0;

    ROOT_NODE = GetStrEnd(tokens, &POS);

    if (tokens[POS].data.oper != END)
        SyntaxErr(tokens, &POS);

    free(tokens);

    return ROOT_NODE;
}

Node *GetStrEnd(struct Token *tokens, size_t *POS) // самая главная, определяет строку
{
    $$$ printf("GetStrEnd\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    Node *new_node = GetFunc(tokens, POS);

    while (tokens[*POS].type == OPER && tokens[*POS].data.oper == STR_END)
    {
        *POS += 1;

        Node *nodeleft = new_node;
        Node *noderight = GetIfTill(tokens, POS);

        new_node = CreateOper(nodeleft, noderight, STR_END);
    }

    return new_node;
}

Node *GetFunc(struct Token *tokens, size_t *POS)
{
    $$$ printf("GetFunc\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    Node *new_node = NULL;

    if (tokens[*POS].type == OPER && tokens[*POS].data.oper == FUNC)
    {
        *POS += 1;
        new_node = GetNumVar(tokens, POS);

        Node *nodeleft = new_node;
        Node *noderight = GetBody(tokens, POS);

        unsigned int count_param = 0;
        CountParam(nodeleft->right, &count_param);

        nodeleft->left = CreateNum(count_param);

        new_node = CreateOper(nodeleft, noderight, FUNC);
    }

    else
        new_node = GetIfTill(tokens, POS);

    return new_node;
}

Node *GetIfTill(struct Token *tokens, size_t *POS)
{
    $$$ printf("GetIfTill\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    Node *new_node = NULL;

    if (tokens[*POS].type == OPER && (tokens[*POS].data.oper == IF ||
                                      tokens[*POS].data.oper == TILL))
    {
        int op = tokens[*POS].data.oper;
        *POS += 1;

        if (op == IF)
        {
            Node *condition = GetEqual(tokens, POS);
            Node *body = GetBody(tokens, POS);
            Node *else_branch = GetElse(tokens, POS);

            new_node = CreateOper(CreateFict(condition, body), else_branch, IF);
        }

        else
        {
            Node *condition = GetEqual(tokens, POS);
            Node *loop_body = GetBody(tokens, POS);

            new_node = CreateOper(condition, loop_body, TILL);
        }
    }

    else
        new_node = GetEqual(tokens, POS);

    return new_node;
}

Node *GetElse(struct Token *tokens, size_t *POS)
{
    $$$ printf("GetElse\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    Node *new_node = NULL;

    if (tokens[*POS].type == OPER && tokens[*POS].data.oper == ELSE)
    {
        *POS += 1;

        Node *nodeleft = NULL;
        Node *noderight = NULL;

        if (tokens[*POS].type == OPER && tokens[*POS].data.oper == IF)
            nodeleft = GetIfTill(tokens, POS);

        else
            noderight = GetBody(tokens, POS);

        new_node = CreateOper(nodeleft, noderight, ELSE);
    }

    else
        new_node = GetBody(tokens, POS);

    return new_node;
}

Node *GetEqual(struct Token *tokens, size_t *POS)
{
    $$$ printf("GetEqual\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    Node *new_node = GetInit(tokens, POS);

    if (tokens[*POS].type == OPER && ( tokens[*POS].data.oper == EQUAL 
                                    || tokens[*POS].data.oper == ABOVE 
                                    || tokens[*POS].data.oper == EQABOVE 
                                    || tokens[*POS].data.oper == BELOW 
                                    || tokens[*POS].data.oper == EQBELOW 
                                    || tokens[*POS].data.oper == NOTEQUAL))
    {
        Operations op = (Operations)(tokens[*POS].data.oper);
        *POS += 1;

        Node *nodeleft = new_node;
        Node *noderight = GetInit(tokens, POS);

        new_node = CreateOper(nodeleft, noderight, op);
    }

    return new_node;
}

Node *GetInit(struct Token *tokens, size_t *POS)
{
    $$$ printf("GetInit\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    Node *new_node = GetExpr(tokens, POS, MIN_PRIORITY);

    if (tokens[*POS].type == OPER && tokens[*POS].data.oper == INIT)
    {
        *POS += 1;

        Node *nodeleft = new_node;
        Node *noderight = GetExpr(tokens, POS, MIN_PRIORITY);

        new_node = CreateOper(nodeleft, noderight, INIT);
    }

    return new_node;
}

Node *GetExpr(Token *tokens, size_t *POS, int min_priority)
{
    Node *left = GetPrimary(tokens, POS);

    if(left == NULL)
        SyntaxErr(tokens, POS);

    while (true)
    {
        Operations oper = tokens[*POS].data.oper;
        int curr_priority = GetPriority(oper);

        if (curr_priority < min_priority)
            break;

        *POS += 1;
        int next_priority = curr_priority + (oper == POW ? 0 : 1);

        Node *right = GetExpr(tokens, POS, next_priority);
        left = CreateOper(left, right, oper);
    }

    return left;
}

Node *GetPrimary(Token *tokens, size_t *POS)
{
    if (tokens[*POS].data.oper == OP_BR)
    {
        *POS += 1;
        Node *node = GetExpr(tokens, POS, MIN_PRIORITY);

        if (tokens[*POS].data.oper != CL_BR)
            SyntaxErr(tokens, POS);

        *POS += 1;
        return node;
    }

    if  (   tokens[*POS].type == OPER
            &&
            (  
                   tokens[*POS].data.oper == SIN 
                || tokens[*POS].data.oper == COS 
                || tokens[*POS].data.oper == TAN 
                || tokens[*POS].data.oper == LOG
            )
        )
    {
        Operations oper = tokens[*POS].data.oper;
        *POS += 1;

        Node* arg = GetPrimary(tokens, POS);
        return CreateOper(NULL, arg, oper);
    }

    else if (tokens[*POS].type == NUM || tokens[*POS].type == VAR)
        return GetNumVar(tokens, POS);

    return NULL;
}

Node *GetNumVar(struct Token *tokens, size_t *POS)
{
    $$$ printf("GetNumVar\n");
    $$$
        MyAssert(tokens, ARG_IN_FUNC);

    if (tokens[*POS].type == VAR)
    {
        *POS += 1;

        Node *node = CreateVar(NULL, NULL, tokens[*POS - 1].data.var);

        if (tokens[*POS].type == OPER && tokens[*POS].data.oper == COMMA)
        {
            *POS += 1;
            node->right = GetNumVar(tokens, POS);
        }

        if (tokens[*POS].type == OPER && tokens[*POS].data.oper == OP_BR)
        {
            node->right = GetExpr(tokens, POS, MIN_PRIORITY);
        }

        return node;
    }

    if (tokens[*POS].type == NUM)
    {
        *POS += 1;

        Node *node = CreateNum(tokens[*POS - 1].data.value);
        return node;
    }

    else
        return NULL;
}

Node *GetBody(struct Token *tokens, size_t *POS) // самая неважная, юзается только после if, till, func
{
    MyAssert(tokens, ARG_IN_FUNC);
    $$$ printf("GetBody %d\n", tokens[*POS].data.oper);
    $$$

        if (tokens[*POS].type == OPER && tokens[*POS].data.oper == OP_BODY)
    {
        $$$ printf("GetBody\n");
        $$$
            *POS += 1;

        Node *node = GetStrEnd(tokens, POS);

        if (tokens[*POS].data.oper != CL_BODY)
            SyntaxErr(tokens, POS);

        *POS += 1;
        return node;
    }

    else return NULL;
}

void SyntaxErr(struct Token *tokens, size_t *POS)
{
    MyAssert(tokens, ARG_IN_FUNC);

    printf("error on: line = %u symbol = %u type = %d, oper = %d\n", tokens[*POS].line, tokens[*POS].symbol, tokens[*POS].type, tokens[*POS].data.oper);

    free(tokens);

    DeleteNode(ROOT_NODE);

    exit(EXIT_FAILURE);
}