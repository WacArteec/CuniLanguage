#include <stdio.h>
#include <string.h>

#include "translator.h"
#include "../Parser/lexical.h"

const char* addict   = "add\n";
const char* subtract = "sub\n";
const char* multiply = "mult\n";
const char* divide   = "idiv\n";
const char* sinus    = "sin\n";
const char* cosinus  = "cos\n";
const char* tangens  = "tan\n";
const char* equal    = "jeq";
const char* noteq    = "jneq";
const char* above    = "jeqb";
const char* below    = "jeqa";
const char* eqabove  = "jb";
const char* eqbelow  = "ja";

unsigned int IFS = 0;
unsigned int TILLS = 0;

void Translator(struct Node* node, struct NameTable* names, FILE* stream)
{
    if(node == NULL)
        return;

    if(node->type == NUM)
    {
        fprintf(stream, "%d\n", node->data.value);
    }

    else if(node->type == OPER)
    {
        const char* action = "";

        if(node->data.oper == STR_END)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);            
        }

        if(node->data.oper == ADD)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = addict;
        }

        else if(node->data.oper == SUB)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = subtract;
        }

        else if(node->data.oper == MULT)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = multiply;
        }

        else if(node->data.oper == DIV)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = divide;
        }

        else if(node->data.oper == SIN)
        {
            Translator(node->right, names, stream);

            action = sinus;
        }

        else if(node->data.oper == COS)
        {
            Translator(node->right, names, stream);

            action = cosinus;
        }

        else if(node->data.oper == TAN)
        {
            Translator(node->right, names, stream);
            action = tangens;
        }

        else if(node->data.oper == INIT)
        {

        }

        else if(node->data.oper == EQUAL)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = equal;
        }

        else if(node->data.oper == NOTEQUAL)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = noteq;
        }

        else if(node->data.oper == EQABOVE)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = eqabove;
        }

        else if(node->data.oper == ABOVE)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = above;
        }

        else if(node->data.oper == EQBELOW)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = eqbelow;
        }

        else if(node->data.oper == BELOW)
        {
            Translator(node->left , names, stream);
            Translator(node->right, names, stream);

            action = below;
        }

        else if(node->data.oper == IF)
        {
            Translator(node->left->left, names, stream); //cmp
            fprintf(stream, " notif%u\n", IFS);//здесь должна быть метка прыжка

            unsigned int cur_if = IFS;
            IFS += 1;

            Translator(node->left->right, names, stream); //body
            fprintf(stream, ":notif%u", cur_if);
            
            if(node->right != NULL)
                Translator(node->right->left, names, stream);// else (пока не придумал)
        }

        else if(node->data.oper == TILL)
        {
            Translator(node->left, names, stream); //cmp
            fprintf(stream, " nottill%u\n", TILLS);//здесь должна быть метка прыжка

            unsigned int cur_till = TILLS;

            Translator(node->right, names, stream); //body
            fprintf(stream, ":nottill%u", cur_till);
        }

        else if(node->data.oper == FUNC)
        {
            if(node->right == NULL)
            {
                fprintf(stream, "push %d \npop BX \n", node->left->left->data.value);
                fprintf(stream, "call %s\n", names->vars[node->left->data.var].name);
                // ещё дописать сдвиг
            }

            else
            {
                Translator(node->right, names, stream);
                fprintf(stream, "ret\n");
            }
            
        }

        fprintf(stream, "%s", action);
    }
}