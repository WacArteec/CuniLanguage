#include "translatorNasm.h"

void TranslateNode(struct Node *node, struct NameTable *names, FILE *stream);
void TranslateNum(struct Node *node, struct NameTable *names, FILE *stream);
void TranslateVar(struct Node *node, struct NameTable *names, FILE *stream);
void TranslateOper(struct Node *node, struct NameTable *names, FILE *stream);
void TranslateSort(struct Node *node, struct NameTable *names, FILE *stream);
void TranslateArgs(Node *node, NameTable *names, FILE *stream);

size_t CNT_MARKS = 0;

void TranslatorNasm(struct Node *root, struct NameTable *names, FILE *stream)
{
    MyAssert(stream, ARG_IN_FUNC);

    fprintf(stream, "section .data\n\n");

    for (size_t i = 0; i < names->size; i++)
        fprintf(stream, "   %s dq %g\n", names->vars[i].name, names->vars[i].init_value);

    fprintf(stream, "section .text\nglobal _start\n\n");

    fprintf(stream, "\tcall main\n");

    fprintf(stream, "\tmov rax, 60\n\txor rdi, rdi\n\tsyscall\n");

    TranslateNode(root, names, stream);
}

void TranslateNode(struct Node *node, struct NameTable *names, FILE *stream)
{
    switch (node->type)
    {
        case NUM:
        {
            TranslateNum(node, names, stream);
            break;
        }

        case OPER:
        {
            TranslateOper(node, names, stream);
            break;
        }
        
        case VAR:
        {
            TranslateVar(node, names, stream);
            break;
        }
        
        case SORT:
        {
            TranslateSort(node, names, stream);
            break;
        }

    default:
        break;
    }
}

void TranslateNum(struct Node *node, struct NameTable *names, FILE *stream)
{
    fprintf(stream, " %g ", node->data.value);
}

void TranslateVar(struct Node *node, struct NameTable *names, FILE *stream)
{
    fprintf(stream, " %s ", names->vars[node->data.var].name);
}

void TranslateSort(struct Node *node, struct NameTable *names, FILE *stream)
{}

void TranslateOper(struct Node *node, struct NameTable *names, FILE *stream)
{
    switch (node->data.oper)
    {
        case INIT:
        {
            fprintf(stream, "\tmov %s, ", names->vars[node->left->data.var].name);
            TranslateNode(node->right, names, stream);

            break;
        }

        case ADD:
        {
            fprintf(stream, "\tmov rax, ");
            TranslateNode(node->left, names, stream);

            fprintf(stream, "\tmov rbx, ");
            TranslateNode(node->right, names, stream);
            
            fprintf(stream, "\tadd rax, rbx\n");

            break;
        }

        case SUB:
        {
            fprintf(stream, "\tmov rax, ");
            TranslateNode(node->left, names, stream);

            fprintf(stream, "\tmov rbx, ");
            TranslateNode(node->right, names, stream);
            
            fprintf(stream, "\tsub rax, rbx\n");

            break;
        }

        case MULT:
        {
            fprintf(stream, "\tmov rax, ");
            TranslateNode(node->left, names, stream);

            fprintf(stream, "\tmov rbx, ");
            TranslateNode(node->right, names, stream);
            
            fprintf(stream, "\timul rax, rbx\n");

            break;
        }

        case DIV:
        {
            fprintf(stream, "\tmov rax, ");
            TranslateNode(node->left, names, stream);

            fprintf(stream, "\tmov rbx, ", names->vars[node->left->data.var].name);
            TranslateNode(node->right, names, stream);

            fprintf(stream, "\txor rdx, rdx\n");
            fprintf(stream, "\tidiv rbx\n");

            break;
        }

        case EQUAL:
        case NOTEQUAL:
        case ABOVE:
        case BELOW:
        case EQABOVE:
        case EQBELOW:
        {
            fprintf(stream, "\tmov rax, ");
            TranslateNode(node->left, names, stream);

            fprintf(stream, "\tmov rbx, ");
            TranslateNode(node->right, names, stream);

            fprintf(stream, "\tcmp rax, rbx\n"); //=(ZF = 1), !=(ZF = 0), <(SF = 1), >(SF = 0)

            break;
        }

        case IF:
        {
            switch (node->left->type)
            {
                case OPER:
                {
                    if (node->left->data.oper > EQBELOW || node->left->data.oper < EQUAL)
                    {
                        TranslateOper(node->left, names, stream);
                        fprintf(stream, "\tcmp rax, 0\n\tjge .NOT_IF%llu", CNT_MARKS);
                    }

                    else
                    {
                        TranslateNode(node->left, names, stream);

                        fprintf(stream, "\t");

                        switch (node->left->data.oper) //здесь всё инвертировано, тк нужно попасть на NOT_IF
                        {
                            case EQUAL:
                            {
                                fprintf(stream, "jne");
                                break;
                            }

                            case NOTEQUAL:
                            {
                                fprintf(stream, "je");
                                break;
                            }

                            case ABOVE:
                            {
                                fprintf(stream, "jl");
                                break;
                            }
                            
                            case BELOW:
                            {
                                fprintf(stream, "jg");
                                break;
                            }
                            
                            case EQABOVE:
                            {
                                fprintf(stream, "jle");
                                break;
                            }
                            
                            case EQBELOW:
                            {
                                fprintf(stream, "jge");
                                break;
                            }
                        
                            default:
                            break;
                        }
                        
                        fprintf(stream, " .NOT_IF%llu\n", CNT_MARKS);
                    }
                }

                case NUM:
                {
                    fprintf(stream, "\tcmp %g, 0\n\tje .NOT_IF%llu", node->left->data.value, CNT_MARKS);

                    break;
                }

                case VAR:
                {
                    fprintf(stream, "\tmov rax, %s\n\tcmp rax, 0\n\tje .NOT_IF%llu", names->vars[node->left->data.var].name, CNT_MARKS);

                    break;
                }

                default:
                    break;
            }

            TranslateNode(node->right, names, stream);
            fprintf(stream, ".NOT_IF:%llu", CNT_MARKS);
            CNT_MARKS++;
        }

        case TILL:
        {
            fprintf(stream, ".TILL:%llu", CNT_MARKS);

            switch (node->left->type)
            {
                case OPER:
                {
                    if (node->left->data.oper > EQBELOW || node->left->data.oper < EQUAL)
                    {
                        TranslateOper(node->left, names, stream);
                        fprintf(stream, "\tcmp rax, 0\n\tjge .NOT_TILL%llu", CNT_MARKS);
                    }

                    else
                    {
                        TranslateNode(node->left, names, stream);

                        fprintf(stream, "\t");

                        switch (node->left->data.oper) //здесь всё инвертировано, тк нужно попасть на NOT_TILL
                        {
                            case EQUAL:
                            {
                                fprintf(stream, "jne");
                                break;
                            }

                            case NOTEQUAL:
                            {
                                fprintf(stream, "je");
                                break;
                            }

                            case ABOVE:
                            {
                                fprintf(stream, "jl");
                                break;
                            }
                            
                            case BELOW:
                            {
                                fprintf(stream, "jg");
                                break;
                            }
                            
                            case EQABOVE:
                            {
                                fprintf(stream, "jle");
                                break;
                            }
                            
                            case EQBELOW:
                            {
                                fprintf(stream, "jge");
                                break;
                            }
                        
                            default:
                            break;
                        }
                        
                        fprintf(stream, " .NOT_TILL%llu\n", CNT_MARKS);
                    }
                }

                case NUM:
                {
                    fprintf(stream, "\tcmp %g, 0\n\tje .NOT_TILL%llu", node->left->data.value, CNT_MARKS);

                    break;
                }

                case VAR:
                {
                    fprintf(stream, "\tmov rax, %s\n\tcmp rax, 0\n\tje .NOT_TILL%llu", names->vars[node->left->data.var].name, CNT_MARKS);

                    break;
                }
                
                default:
                    break;
            }

            TranslateNode(node->right, names, stream);

            fprintf(stream, "jmp .TILL%llu", CNT_MARKS);
            fprintf(stream, ".NOT_TILL:%llu", CNT_MARKS);

            CNT_MARKS++;
        }

        case FUNC:
        {
            fprintf(stream, ".%s:", names->vars[node->left->data.var].name);
            TranslateNode(node->right, names, stream);

            fprintf(stream, "ret\n");

            break;
        }
    
        default:
            break;
    }
}

void TranslateArgs(Node *node, NameTable *names, FILE *stream)
{
    fprintf(stream, "");
}