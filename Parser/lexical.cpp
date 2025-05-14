#include "lexical.h"

unsigned int WordsCounter(char *str);
unsigned int SymbolsCounter(char *str);
void LexicErr(unsigned int line, unsigned int symbol);

unsigned int CUR_VAR = 0;
unsigned int scope = 0;

Token *LexicalAnalis(char *text, struct NameTable *names)
{
    names->vars = (VarTable *)calloc(DEF_SIZE, sizeof(VarTable));
    MyAssert(names->vars, ALLOC);

    names->size = DEF_SIZE;

    unsigned int count_words = WordsCounter(text) + 1;
    $$$ printf("Count of words = %u\n", count_words);

    Token *tokens = (Token *)calloc(count_words, sizeof(Token));
    MyAssert(tokens, ALLOC);

    unsigned int count_tokens = 0;

    unsigned int text_size = SymbolsCounter(text);
    $$$ printf("Count of symbols = %u\n", text_size);

    unsigned int line = 1;
    unsigned int symbol = 1;

    for (unsigned int i = 0; i < text_size; i++)
    {
        $$$ printf("BIG BLACK COMMENT%d\n", i);
        symbol += 1;

        if (text[i] == ' ')
            i += 1;

        else if (text[i] == '\t')
        {
            symbol += 3;
            i += 4;
        }

        else
        {
            tokens[count_tokens].line = line;
            tokens[count_tokens].symbol = symbol;

            if (isdigit(text[i]) != 0)
            {
                tokens[count_tokens].type = NUM;

                char *endp = NULL;

                tokens[count_tokens].data.value = strtod(text + i, &endp);

                $$$ printf("SdVig izza chisla = %u\n"), (unsigned int)(endp - text) - i - 1;

                i += (unsigned int)(endp - text) - i - 1;
            }

            else if (isalpha(text[i]) != 0)
            {
                $$$ printf("isalpha COMMENT%d\n", i);
                if (strncmp(text, "sin", sizeof("sin") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = SIN;
                    i += 2;
                }

                else if (strncmp(text, "log", sizeof("log") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = LOG;
                    i += 2;
                }

                else if (strncmp(text, "cos", sizeof("cos") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = COS;
                    i += 2;
                }

                else if (strncmp(text, "tan", sizeof("tan") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = TAN;
                    i += 2;
                }

                else if (strncmp(text, "if", sizeof("if") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = IF;
                    i += 1;
                }

                else if (strncmp(text, "else", sizeof("else") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = ELSE;
                    i += 3;
                }

                else if (strncmp(text, "till", sizeof("till") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = TILL;
                    i += 3;
                }

                else if (strncmp(text, "func", sizeof("func") / sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = FUNC;
                    i += 3;
                }

                else
                {
                    $$$ printf("var COMMENT%d\n", i);

                    if (names->size == CUR_VAR + 1)
                    {
                        names->vars = (VarTable *)realloc(names->vars, 2 * names->size * sizeof(NameTable));
                        MyAssert(names->vars, ALLOC);
                    }

                    names->vars[CUR_VAR].name = (char *)calloc(DEF_SIZE, sizeof(char));
                    MyAssert(names->vars[CUR_VAR].name, ALLOC);

                    unsigned int shift = 0;

                    while (isalpha(text[i + shift]) != 0 || text[i + shift] == '_')
                    {
                        $$$ printf("var1 COMMENT%d\n", i + shift);
                        names->vars[CUR_VAR].name[shift] = text[i + shift];

                        shift += 1;

                        if (shift == sizeof(names->vars[CUR_VAR].name))
                        {
                            realloc(names->vars[CUR_VAR].name, 2 * shift * sizeof(char));
                            MyAssert(names->vars[CUR_VAR].name, ALLOC);
                        }
                    }

                    i += shift - 1;
                    $$$ printf("varEnd COMMENT%d\n", i);
                    //$$$ break;

                    tokens[count_tokens].type = VAR;
                    tokens[count_tokens].data.var = CUR_VAR;

                    names->vars[CUR_VAR].scope = scope;
                    names->vars[CUR_VAR].order = CUR_VAR;

                    CUR_VAR += 1;
                }
            }

            else if (text[i] == '\r')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = STR_END;

                line += 1;
                symbol = 0;
                i += 1;
            }

            else if (text[i] == '(')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = OP_BR;
            }

            else if (text[i] == ')')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = CL_BR;
            }

            else if (text[i] == ':')
            {
                $$$ printf("init i = %u \n", i);
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = INIT;
            }

            else if (text[i] == '!' && text[i + 1] == '=')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = NOTEQUAL;

                i += 1;
            }

            else if (text[i] == '=')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = EQUAL;
            }

            else if (text[i] == '>' && text[i + 1] == '=')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = EQABOVE;

                i += 1;
            }

            else if (text[i] == '>')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = ABOVE;
            }

            else if (text[i] == '<' && text[i + 1] == '=')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = EQBELOW;

                i += 1;
            }

            else if (text[i] == '<')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = BELOW;
            }

            else if (text[i] == ',')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = COMMA;
            }

            else if (text[i] == '+')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = ADD;
            }

            else if (text[i] == '-')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = SUB;
            }

            else if (text[i] == '*')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = MULT;
            }

            else if (text[i] == '/')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = DIV;
            }

            else if (text[i] == '^')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = POW;
            }

            else if (text[i] == ':')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = INIT;
            }

            else if (text[i] == '}')
            {
                scope += 1;

                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = OP_BODY;
            }

            else if (text[i] == '{')
            {
                scope += 1;

                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = CL_BODY;
            }

            else if (text[i] == '\0')
            {
                $$$ printf("end\n");
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = END;
            }

            else
                LexicErr(line, symbol);

            count_tokens += 1;
        }
    }

    names->vars = (VarTable *)realloc(names->vars, (CUR_VAR + 1) * sizeof(VarTable));
    MyAssert(names->vars, ALLOC);

    $$$ printf("count_tokens = %u\n", count_tokens);
    return tokens;
}

unsigned int WordsCounter(char *str)
{
    unsigned int count = 1;
    unsigned int pos = 1;

    while (str[pos] != '\0')
    {

        if ((isdigit(str[pos - 1]) != isdigit(str[pos]) ||
             isalpha(str[pos - 1]) != isalpha(str[pos])) &&
            str[pos] != ' ' && str[pos] != '.' && str[pos - 1] != '.')
        {
            count += 1;
        }

        pos += 1;
    }

    return count;
}

unsigned int SymbolsCounter(char *str)
{
    unsigned int count = 1;
    unsigned int pos = 0;

    while (str[pos] != '\0')
    {
        count += 1;
        pos += 1;
    }

    return count;
}

void LexicErr(unsigned int line, unsigned int symbol)
{
    printf("Lexical error on: line = %u, symbol = %u\n", line, symbol);

    exit(1);
}