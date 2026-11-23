#include "h/io.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/tree.h"
#include "h/utils.h"
#include <cstdio>
#include <cstring>

#define SWITCH_SIDE(__curSide)                                                                                         \
    {                                                                                                                  \
        if ((__curSide) == tcsLeft)                                                                                    \
        {                                                                                                              \
            (__curSide) = tcsRight;                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            (__curSide) = tcsLeft;                                                                                     \
        }                                                                                                              \
    }

Error LoadStrTreeFromBuf(Tree *const tree, char *buf)
{
    ASSERT(tree);
    ASSERT(buf);

    ReplaceAllSpaceCharWithNullTerminator(buf);
    // (\0бм\0(\0биба\0)\0(\0боба\0)\0)\0\0

    char **ptr = &buf;
    // PRINTF_DEBUG("ptr = <%s>\n", ptr);
    *ptr += strlen(*ptr) + 1;
    DeleteNode(tree->root);
    RETURN_ERROR_IF_FAIL(NewNode(&tree->root, *ptr));
    // PRINTF_DEBUG("ptr = <%s>\n", ptr);
    *ptr += strlen(*ptr) + 1;

    return LoadNodesFromStr(tree->root, ptr, tcsLeft);

    // while (*ptr != '\0')
    // {

    //         if (*ptr == '(' || *ptr == ')')
    //         {
    //             ptr += strlen(ptr);
    //             continue;
    //         }
    //
    //         PRINTF_DEBUG("got str <%s>\n", ptr);
    //         AddToTree(tree, ptr);
    //         ptr += strlen(ptr);
    // }
    //     if (*ptr == '\0')
    //     {
    //         PRINTF_DEBUG("*ptr == \\0;\n");
    //         return cgSuccessErrorStruct;
    //     }
    //
    //     PRINTF_DEBUG("skipped return in if, error. ptr = <%s>, *ptr = <%c>\n", ptr, *ptr);
    //     return CreateError(ecUnexpectedFailure, "");
}

Error LoadNodesFromStr(Node *const root, char **ptr, TreeChildSide curSide) // FIXME !!!
{
    ASSERT(root);
    ASSERT(ptr);
    ASSERT(*ptr);

    PRINTF_DEBUG("*ptr = <%s>\n", *ptr);

    Node **newNodePlace = curSide == tcsLeft ? &root->left : &root->right;

    switch (**ptr)
    {
    case '\0':
        return cgSuccessErrorStruct;

    case '(':
        if (*(*ptr + 2) == ')')
        {
            *ptr += 4;
            *newNodePlace = NULL;
        }
        else
        {
            *ptr += strlen(*ptr) + 1;
            RETURN_ERROR_IF_FAIL(NewNode(newNodePlace, *ptr));
            *ptr += strlen(*ptr) + 1;

            RETURN_ERROR_IF_FAIL(LoadNodesFromStr(*newNodePlace, ptr, tcsLeft));
        }

        if (curSide == tcsLeft)
        {
            return LoadNodesFromStr(root, ptr, tcsRight);
        }

        if (**ptr == ')')
        {
            *ptr += strlen(*ptr) + 1;
        }

        return cgSuccessErrorStruct;

    case ')':
        *ptr += strlen(*ptr) + 1;
        return cgSuccessErrorStruct;

    default:
        return CreateError(ecParsingFailed, *ptr);
    }
}
