#include "h/io.h"
#include "h/constants.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/tree.h"
#include "h/utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Error LoadStrTreeFromBuf(Tree *const tree, const char *const buf)
{
    ASSERT(tree);
    ASSERT(buf);

    const char *ptr = buf;
}

Error LoadStrNodesFromFile(Node *const node, const char **ptr)
{
    //     ASSERT(node);
    //     ASSERT(ptr);
    //     ASSERT(*ptr);
    //
    //     Node *nodeToWrite = NULL;
    //
    //     int distToOpenBrace = strchr(*ptr, '(') - *ptr;
    //     int distToClosedBrace = strchr(*ptr, ')') - *ptr;
    //
    //     size_t strToWriteLen = MIN(distToOpenBrace, distToClosedBrace);
    //     char *strToWrite = (char *)calloc(cNodeDataStrMaxSize, sizeof(char));
    //
    //     strncpy(strToWrite, *ptr, strToWriteLen);
    //     *ptr += strToWriteLen;
    //
    //     NewNode(&nodeToWrite, strToWrite);
}
