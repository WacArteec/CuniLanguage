#ifndef MYASSERT_H
#define MYASSERT_H

#include <stdio.h>
#include <stdlib.h>

typedef enum RUNTIME_ERROR
{
    ALLOC,
    FILES,
    ARG_IN_FUNC
} RUNTIME_ERROR;

void MyAssert(void *need_check, RUNTIME_ERROR re_error);

#endif //MYASSERT_H