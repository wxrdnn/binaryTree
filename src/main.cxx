#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/graphing.h"
#include "h/tree.h"
#include <cstdio>

#define SHUTDOWN(__exitCode)                                                                                           \
    {                                                                                                                  \
        if (tree)                                                                                                      \
        {                                                                                                              \
            DeleteTree(tree);                                                                                          \
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

#define OUTPUT_FILE_NAME "../logs/" __DATE__ "_" __TIME__ ".gv"

int IntComparator(const void *a, const void *b);
void IntToStr(const NodeData_t data, char *const out, const size_t outSize);

int main()
{
    Tree *tree = NULL;
    SHUTDOWN_IF_ERROR_FAIL(NewTree(&tree, 50, IntComparator, IntToStr));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 40));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 30));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 45));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 60));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 55));
    SHUTDOWN_IF_ERROR_FAIL(AddToTree(tree, 65));

    char buf[1024] = {};
    char *cur = buf;
    SHUTDOWN_IF_ERROR_FAIL(DumpNodes(tree->root, buf, &cur, 1024, IntToStr));
    fprintf(stderr, "DumpNodes: %s\n", buf);

    FILE *graphOutput = fopen(OUTPUT_FILE_NAME, "w");
    BuildGraph(tree, graphOutput);
    fclose(graphOutput);

    int status = system("dot -Tpng \"" OUTPUT_FILE_NAME "\" -o \"" OUTPUT_FILE_NAME ".png\"");

    // fprintf(stderr, "Left = %p, Right = %p\n", tree->root->left, tree->root->right);

    SHUTDOWN(ecSuccess);
}

int IntComparator(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

void IntToStr(const NodeData_t data, char *const out, const size_t outSize)
{
    ASSERT(out);
    snprintf(out, outSize - 1, "%d", data);
    return;
}
