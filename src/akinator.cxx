#include "h/akinatot.h"
#include "h/debug.h"
#include <cstdio>

TreeChildSide AskQuestion(Node *const root)
{
    ASSERT(root);

    printf("%s\n", root->data);

    char ansChar = '\0';

    while (ansChar != 'y' && ansChar != 'n')
    {
        printf("[y/n]: ");
        scanf("%c", &ansChar);
        putc('\n', stdout);
    }

    return ansChar == 'y' ? tcsRight : tcsLeft;
}
