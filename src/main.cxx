#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/tree.h"
#include <cstdio>

#define SHUTDOWN(__exitCode)                                                                                           \
    {                                                                                                                  \
        if (tree)                                                                                                      \
        {                                                                                                              \
        }                                                                                                              \
        return __exitCode;                                                                                             \
    }

#define SHUTDOWN_IF_ERROR_FAIL(__error)                                                                                \
    {                                                                                                                  \
        Error __err = __error;                                                                                         \
        if (__err.exitCode != ecSuccess)                                                                               \
        {                                                                                                              \
            HandleError(__err);                                                                                        \
            SHUTDOWN(__err.exitCode);                                                                                  \
        }                                                                                                              \
    }

int IntComparator(const void *a, const void *b);
void IntToStr(const TreeElement_t data, char *const out, const size_t outSize);

int main()
{
    Tree *tree = NULL;
    SHUTDOWN_IF_ERROR_FAIL(NewTree(&tree, 0, IntComparator, IntToStr));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 10));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 30));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 40));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 20));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, -10));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, -50));

    char buf[4096] = {};
    char *cur = buf;
    SHUTDOWN_IF_ERROR_FAIL(DumpNodes(tree->root->left, buf, &cur, 4096, IntToStr));
    fprintf(stderr, "DumpNodes: %s\n", buf);
    // fprintf(stderr, "Left = %p, Right = %p\n", tree->root->left, tree->root->right);

    SHUTDOWN(ecSuccess);
}

int IntComparator(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

void IntToStr(const TreeElement_t data, char *const out, const size_t outSize)
{
    ASSERT(out);
    snprintf(out, outSize - 1, "%d", data);
    return;
}
