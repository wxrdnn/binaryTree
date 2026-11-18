#ifndef GRAPHING_H

#define GRAPHING_H

#include "errorHandle.h"
#include "stdio.h"
#include "tree.h"

Error BuildGraph(const Tree *const tree, FILE *const output);

Error BuildNodes(const Node *const node, FILE *const output,
                 void (*nodeDataToStr)(const NodeData_t data, char *const out, const size_t outSize));

#endif
