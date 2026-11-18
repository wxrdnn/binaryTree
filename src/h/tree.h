#ifndef TREE_H

#define TREE_H

#include "errorHandle.h"
#include <cstddef>
typedef int TreeElement_t;

struct Node
{
    Node *left;
    Node *right;
    TreeElement_t data;

    // meta
    int _level;
};

struct Tree
{
    Node *root;
    int (*comp)(const void *, const void *);
    void (*nodeDataToStr)(const TreeElement_t data, char *const out, const size_t outSize);

    // meta
    size_t elementAmount;
    unsigned maxLevel;
};

Error NewNode(Node **nodePtr, TreeElement_t data);

void DeleteNode(Node *node);

Error NewTree(Tree **treePtr, TreeElement_t rootData, int (*comp)(const void *, const void *),
              void (*nodeDataToStr)(const TreeElement_t data, char *const out, const size_t outSize));

Error AddToTree(Tree *const tree, const TreeElement_t data);

Node *FindInTree(Tree *const tree, const TreeElement_t dataToSearch, Node **prevOfFoundNode);

Error DumpNodes(const Node *node, char *const out, char **curPtr, const size_t outSize,
                void (*nodeDataToStr)(const TreeElement_t data, char *out, const size_t outSize));

#endif
