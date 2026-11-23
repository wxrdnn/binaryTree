#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/graphing.h"
#include "h/io.h"
#include "h/tree.h"
#include "h/utils.h"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#define SHUTDOWN(__exitCode)                                                                                           \
    {                                                                                                                  \
        if (tree)                                                                                                      \
        {                                                                                                              \
            DeleteTree(tree);                                                                                          \
        }                                                                                                              \
        if (inputBuf)                                                                                                  \
        {                                                                                                              \
            free(inputBuf);                                                                                            \
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
void StrToStr(const NodeData_t data, char *const out, const size_t outSize);

Error _CreateAndLoadTree(Tree **tree, char *inputBuf);

Error _DumpTree(Tree *tree, FILE *output);

void DeleteStr(NodeData_t data);

int main()
{
    Tree *tree = NULL;
    char *inputBuf = NULL;

    SHUTDOWN_IF_ERROR_FAIL(_CreateAndLoadTree(&tree, inputBuf));

    FILE *graphOutput = fopen(OUTPUT_FILE_NAME, "w");
    BuildGraph(tree, graphOutput);
    fclose(graphOutput);

    int status = system("dot -Tpng \"" OUTPUT_FILE_NAME "\" -o \"" OUTPUT_FILE_NAME ".png\"");

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

void StrToStr(const NodeData_t data, char *const out, const size_t outSize)
{
    ASSERT(out);
    strncpy(out, data, outSize - 1);
    return;
}

void DeleteStr(NodeData_t data)
{
    ASSERT(data);

    // free(data);
    return;
}

Error _CreateAndLoadTree(Tree **tree, char *inputBuf)
{
    ASSERT(tree);

    int inputfd = open("akinatorTree.txt", O_RDONLY);

    RETURN_ERROR_IF_FAIL(NewTree(tree, "root", StrToStr, DeleteStr));

    if (inputfd == 0)
    {
        return CreateError(TranslateErrnoCode(errno), "akinatorTree.txt");
    }

    size_t inputBufSize = 0;
    RETURN_ERROR_IF_FAIL(GetFileSize("akinatorTree.txt", &inputBufSize));

    RETURN_ERROR_IF_FAIL(CreateTextBuf(inputBufSize + 1, &inputBuf));

    RETURN_ERROR_IF_FAIL(LoadText(inputfd, inputBuf, inputBufSize));

    ReplaceAllChars(inputBuf, '|', '\0');

    RETURN_ERROR_IF_FAIL(LoadStrTreeFromBuf(*tree, inputBuf));

    return cgSuccessErrorStruct;
}

Error _DumpTree(Tree *tree, FILE *output)
{
    ASSERT(tree);
    ASSERT(output);

    char dumpBuf[1024] = {};
    char *cur = dumpBuf;
    RETURN_ERROR_IF_FAIL(DumpNodes(tree->root, dumpBuf, &cur, 1024, tree->NodeDataToStr));
    fprintf(output, "%s\n", dumpBuf);

    return cgSuccessErrorStruct;
}
