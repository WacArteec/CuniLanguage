#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "../Parser/lexical.h"

void Translator(struct Node* node, struct NameTable* names, FILE* stream);

#endif //TRANSLATOR_H