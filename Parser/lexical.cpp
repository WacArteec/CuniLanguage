#include "lexical.h"

unsigned int WordsCounter(const char *str);
unsigned int SymbolsCounter(char *str);
void LexicErr(unsigned int line, unsigned int symbol);

Token *LexicalAnalis(char *text, struct NameTable *names)
{
    names->vars = (VarTable *)calloc(DEF_SIZE, sizeof(VarTable));
    MyAssert(names->vars, ALLOC);

    names->size = DEF_SIZE;

    unsigned int count_words = WordsCounter(text) + 1;
    $$$ printf("Count of words = %u\n", count_words); $$$

    Token *tokens = (Token *)calloc(count_words, sizeof(Token));
    MyAssert(tokens, ALLOC);

    unsigned int count_tokens = 0;

    unsigned int text_size = SymbolsCounter(text);
    $$$ printf("Count of symbols = %u\n", text_size);

    unsigned int line = 1;
    unsigned int symbol = 1;

    unsigned int CUR_VAR = 0;
    unsigned int scope = 0;

    for (unsigned int i = 0; i < text_size; i++)
    {
        //$$$ printf("BIG BLACK COMMENT%d\n", i);
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
                char *endp = NULL;
                double value = strtod(text + i, &endp);

                if (endp == text + i)
                    LexicErr(line, symbol);

                const char *ptr = text + i;
                bool has_dot = false;

                while (ptr < endp)
                {
                    if (*ptr == '.')
                    {
                        if (has_dot)
                            LexicErr(line, symbol + (ptr - (text + i)));

                        has_dot = true;
                    }

                    ptr++;
                }

                if (*endp != '\0' && !(isspace(*endp) || strchr("+-*/=<>(){}[],;", *endp) != NULL))
                    LexicErr(line, symbol + (endp - (text + i)));

                tokens[count_tokens].type = NUM;
                tokens[count_tokens].data.value = value;

                i += (endp - (text + i)) - 1; 
            }

            else if (isalpha(text[i]) != 0)
            {
                $$$ printf("isalpha COMMENT%d %c\n", i, text[i]);
                if (strncmp(text + i, "sin", sizeof("sin") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("sin")]))
                {
                    $$$ printf("\tsin\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = SIN;
                    i += strlen("sin") - 1;
                }

                else if (strncmp(text + i, "log", sizeof("log") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("log")]))
                {
                    $$$ printf("\tlog\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = LOG;
                    i += strlen("log") - 1;
                }

                else if (strncmp(text + i, "cos", sizeof("cos") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("log")]))
                {
                    $$$ printf("\tcos\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = COS;
                    i += strlen("cos") - 1;
                }

                else if (strncmp(text + i, "tan", sizeof("tan") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("log")]))
                {
                    $$$ printf("\ttan\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = TAN;
                    i += strlen("tan") - 1;
                }

                else if (strncmp(text + i, "if", sizeof("if") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("if")]))
                {
                    $$$ printf("\tif\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = IF;
                    i += strlen("if") - 1;
                }

                else if (strncmp(text + i, "else", sizeof("else") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("else")]))
                {
                    $$$ printf("\telse\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = ELSE;
                    i += strlen("else") - 1;
                }

                else if (strncmp(text + i, "till", sizeof("till") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("till")]))
                {
                    $$$ printf("\ttill\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = TILL;
                    i += strlen("till") - 1;
                }

                else if (strncmp(text + i, "func", sizeof("func") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("func")]))
                {
                    $$$ printf("\tfunc\n");
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].data.oper = FUNC;
                    i += strlen("func");
                }

                else if (strncmp(text + i, "int", sizeof("int") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("int")]))
                {
                    $$$ printf("\tint\n");
                    tokens[count_tokens].type = SORT;
                    tokens[count_tokens].data.sort = INT;
                    i += strlen("int") - 1;
                }

                else if (strncmp(text + i, "void", sizeof("void") / sizeof(char) - 1) == 0 && !isalnum(text[i + strlen("void")]))
                {
                    $$$ printf("\tvoid\n");
                    tokens[count_tokens].type = SORT;
                    tokens[count_tokens].data.sort = VOID;
                    i += strlen("void") - 1;
                }

                else
                {
                    $$$ printf("var COMMENT%d %u\n", i, CUR_VAR);

                    if (names->size == CUR_VAR + 1)
                    {
                        names->vars = (VarTable *)realloc(names->vars, 2 * names->size * sizeof(VarTable));
                        MyAssert(names->vars, ALLOC);

                        names->size *= 2;
                    }
                    $$$ printf("var COMMENT%d %u %llu %p\n", i, CUR_VAR, names->size, names->vars);

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

                    for (size_t i = 0; i < CUR_VAR; i++)
                    {
                        if (strcmp(names->vars[i].name, names->vars[CUR_VAR].name) == 0 && names->vars[i].scope == names->vars[CUR_VAR].scope)
                        {
                            free(names->vars[CUR_VAR].name);
                            names->vars[CUR_VAR].name = NULL;

                            CUR_VAR--;

                            break;
                        }
                    }
                    
                    if (tokens[count_tokens - 1].type == OPER && tokens[count_tokens - 1].data.oper == FUNC)
                        names->vars[CUR_VAR].sort = IS_FUNC;

                    else if (tokens[count_tokens - 1].type == SORT)
                        names->vars[CUR_VAR].sort = tokens[count_tokens].data.sort;

                    i += shift - 1;
                    $$$ printf("varEnd COMMENT%d\n", i);
                    //$$$ break;

                    tokens[count_tokens].type = VAR;
                    tokens[count_tokens].data.var = CUR_VAR;

                    names->vars[CUR_VAR].scope = scope;
                    names->vars[CUR_VAR].order = CUR_VAR;

                    CUR_VAR++;
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

            else if (text[i] == '\0')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = END;

                break;
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

            else if (text[i] == '{')
            {
                scope += 1;

                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = OP_BODY;
            }

            else if (text[i] == '}')
            {
                scope += 1;

                tokens[count_tokens].type = OPER;
                tokens[count_tokens].data.oper = CL_BODY;
            }

            else if (text[i] == '#')
            {
                while (text[i] != '\r')
                i++;

                i--;
            }

            else if (text[i] == '\0')
            {
                $$$ printf("end\n");
                break;
            }

            else
            {
                printf("symbol:%c%d\n", text[i], tokens[count_tokens].data.oper);
                LexicErr(line, symbol);
            }

            count_tokens += 1;
        }
    }

    names->vars = (VarTable *)realloc(names->vars, (CUR_VAR + 1) * sizeof(VarTable));
    MyAssert(names->vars, ALLOC);

    $$$ printf("count_tokens = %u\n", count_tokens);

    for (size_t i = 0; i < count_tokens; i++)
    {
        printf("(%lu %d %d %g)\n", i, tokens[i].type, tokens[i].data.oper, tokens[i].data.value);
    }
    

    return tokens;
}

unsigned int WordsCounter(const char *str)
{
    enum State
    {
        STATE_DEFAULT,
        STATE_NUMBER,
        STATE_IDENTIFIER,
        STATE_OPERATOR
    };

    State current_state = STATE_DEFAULT;
    unsigned int count = 0;
    size_t i = 0;

    while (str[i] != '\0')
    {
        if (isspace(str[i]))
        {
            i++;
            continue;
        }

        if (isdigit(str[i]) || (str[i] == '.' && current_state == STATE_NUMBER))
        {
            if (current_state != STATE_NUMBER)
            {
                count++;
                current_state = STATE_NUMBER;
            }
            i++;
        }

        else if (isalpha(str[i]) || str[i] == '_')
        {
            if (current_state != STATE_IDENTIFIER)
            {
                count++;
                current_state = STATE_IDENTIFIER;
            }

            i++;
        }

        else if (strchr("+-*/=!<>", str[i]))
        {
            if (str[i + 1] == '=')
            {
                count++;
                i += 2;
            }

            else
            {
                count++;
                i++;
            }

            current_state = STATE_OPERATOR;
        }

        else
        {
            count++;
            current_state = STATE_DEFAULT;
            i++;
        }
    }

    return count;
}

/*unsigned int WordsCounter(char *str)
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
}*/

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
    fprintf(stderr, "Lexical error on: line = %u, symbol = %u\n", line, symbol);

    exit(EXIT_FAILURE);
}

void NameTableDtor(NameTable *names)
{
    for (size_t i = 0; i < names->size; i++)
        free(names->vars[i].name);

    free(names->vars);
    names->vars = NULL;

    names->size = 0;
}