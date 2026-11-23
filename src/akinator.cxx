#include "h/akinatot.h"
#include "h/constants.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/tree.h"
#include "h/utils.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

Error AskQuestions(Node *const root)
{
    ASSERT(root);

    printf("%s?\n", root->data);

    char ansChar = 'd';

    while (ansChar != 'y' && ansChar != 'n')
    {
        // PRINTF_DEBUG("iteration, ansChar = %c\n", ansChar);
        printf("[y/n]: ");
        scanf("%c", &ansChar);
        putc('\n', stdout);
        ClearBuffer();
    }

    TreeChildSide ans = ansChar == 'y' ? tcsRight : tcsLeft;
    Node *nextNode = ans == tcsLeft ? root->left : root->right;
    if (nextNode != NULL)
    {
        RETURN_ERROR_IF_FAIL(AskQuestions(nextNode));
    }
    else
    {
        if (ansChar == 'y')
        {
            printf("Изи\n");
        }
        else
        {
            printf("Тогда кто это был?\n");
            char *newData = (char *)calloc(cNodeDataStrMaxSize, sizeof(char));

            scanf("%127[^\n]", newData);
            ClearBuffer();
            ReplaceChar(newData, '\n', '\0');

            printf("Чем %s отличается от %s? Он: ", newData, root->data);
            char *newQuestion = (char *)calloc(cNodeDataStrMaxSize, sizeof(char));
            scanf("%127[^\n]", newQuestion);
            ClearBuffer();
            ReplaceChar(newQuestion, '\n', '\0');

            NodeData_t curData = root->data;
            root->data = newQuestion;

            RETURN_ERROR_IF_FAIL(NewNode(&root->right, newData));
            RETURN_ERROR_IF_FAIL(NewNode(&root->left, curData));
        }
    }
    return cgSuccessErrorStruct;
}
