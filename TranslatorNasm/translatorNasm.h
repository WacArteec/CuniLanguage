#ifndef TRANSLATORNASM_H
#define TRANSLATORNASM_H

#include "../tree/tree.h"
#include "../Parser/lexical.h"

void TranslatorNasm(struct Node *root, struct NameTable *names, FILE *stream);

#endif //TRANSLATORNASM_H