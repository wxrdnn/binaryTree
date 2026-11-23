#ifndef IO_H

#define IO_H

#include "tree.h"
#include <cstdio>

Error LoadStrTreeFromBuf(Tree *const tree, char *const buf);

Error LoadNodesFromStr(Node *const root, char **ptr, TreeChildSide curSide);

#endif
