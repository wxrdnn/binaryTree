#ifndef IO_H

#define IO_H

#include "tree.h"
#include <cstdio>

Error LoadStrTreeFromBuf(Tree *const tree, const char *buf);

Error LoadNodesFromStr(Node *const root, const char **ptr, TreeChildSide curSide);

Error GetFileSize(const char *const path, size_t *size);

Error LoadText(const int fd, char *const textBuf, const size_t fileSize); // needs textBuf size of <fileSize + 1>

Error CreateTextBuf(const size_t size, char **textBuf);

#endif
