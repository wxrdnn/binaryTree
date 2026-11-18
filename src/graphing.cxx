#include "h/graphing.h"
#include "h/constants.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include <cstdio>

Error BuildGraph(const Tree *const tree, FILE *const output)
{
    ASSERT(tree);
    ASSERT(output);

    fprintf(output,
            "digraph G {\n"
            "nodesep=1;\n"
            // "ranksep=1;\n"
            // "splines=ortho;\n"
            "subgraph List {\n"
            // "rank=same;\n"
    );

    BuildNodes(tree->root, output, tree->nodeDataToStr);

    fprintf(output, "}\n");

    fprintf(output, "}\n");

    return cgSuccessErrorStruct;
}

Error BuildNodes(const Node *const node, FILE *const output,
                 void (*nodeDataToStr)(const NodeData_t data, char *const out, const size_t outSize))
{
    // TODO Error handle
    ASSERT(node);
    ASSERT(output);

    fprintf(output, "node_%p [\n", node);

    fprintf(output,
            "shape=Mrecord;\n"
            "style=\"filled\";\n"
            "fillcolor=\"#C0FFC0\";\n"
            "color=\"#008000\";\n"
            "weight=50;\n");

    char nodeDataStr[cNodeDataStrMaxSize] = {};
    nodeDataToStr(node->data, nodeDataStr, cNodeDataStrMaxSize);

    fprintf(output,
            "label=\"{ %p | data = %s | { left = %p | right = %p } }\";\n",
            node,
            nodeDataStr,
            node->left,
            node->right);

    fprintf(output, "];\n");

    if (node->left)
    {
        fprintf(output, "node_%p -> node_%p\n", node, node->left);
        RETURN_ERROR_IF_FAIL(BuildNodes(node->left, output, nodeDataToStr));
    }

    if (node->right)
    {
        fprintf(output, "node_%p -> node_%p\n", node, node->right);
        RETURN_ERROR_IF_FAIL(BuildNodes(node->right, output, nodeDataToStr));
    }

    return cgSuccessErrorStruct;
}
