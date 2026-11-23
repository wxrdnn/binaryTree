#include "h/io.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/tree.h"
#include "h/utils.h"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

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

Error LoadStrTreeFromBuf(Tree *const tree, const char *buf)
{
    ASSERT(tree);
    ASSERT(buf);

    // ReplaceAllSpaceCharWithNullTerminator(buf);
    // (\0бм\0(\0биба\0)\0(\0боба\0)\0)\0\0

    const char **ptr = &buf;
    // PRINTF_DEBUG("ptr = <%s>\n", ptr);
    *ptr += strlen(*ptr) + 1;
    DeleteNode(tree->root);
    RETURN_ERROR_IF_FAIL(NewNode(&tree->root, (char *)*ptr));
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

Error LoadNodesFromStr(Node *const root, const char **ptr, TreeChildSide curSide)
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
        }
        else
        {
            *ptr += strlen(*ptr) + 1;
            RETURN_ERROR_IF_FAIL(NewNode(newNodePlace, (char *)*ptr));
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

Error GetFileSize(const char *const path, size_t *size)
{
    ASSERT(path);
    ASSERT(size);

    struct stat buf = {};
    int errnoCode = stat(path, &buf);
    Error error = CreateError(TranslateErrnoCode(errnoCode), path);
    *size = (size_t)buf.st_size;
    return error;
}

Error LoadText(const int fd, char *const textBuf, const size_t fileSize) // needs textBuf size of <fileSize + 1>
{
    ASSERT(textBuf);

    long int numberRead = read(fd, textBuf, fileSize);
    Error error = CreateError(ecSuccess, "");
    if (numberRead != (long int)fileSize)
    {
        error.exitCode = ecCantReadFile;
        return error;
    }

    textBuf[fileSize] = '\0';

    return error;
}

Error CreateTextBuf(const size_t size, char **textBuf)
{
    ASSERT(textBuf);

    *textBuf = (char *)calloc(size, sizeof(char));
    Error error = CreateError(ecSuccess, "");
    if (!*textBuf)
    {
        error = CreateError(ecCantAllocateMemory, "text buffer");
        return error;
    }
    return error;
}
