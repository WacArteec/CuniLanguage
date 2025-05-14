#include "myassert.h"

void MyAssert(void *need_check, RUNTIME_ERROR rt_error)
{
    if(need_check)
        return;

    switch (rt_error)
    {
        case ALLOC:
        {
            fprintf(stderr, "allocation failed. file:%s line: %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);

            break;
        }
        
        case FILES:
        {
            fprintf(stderr, "file not opened. file:%s line: %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);

            break;
        }

        case ARG_IN_FUNC:
        {
            fprintf(stderr, "argument in function is not correct. file:%s line: %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);

            break;
        }

    default:
        break;
    }
}