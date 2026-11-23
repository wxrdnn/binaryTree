#ifndef TREE_H

#define TREE_H

#include "errorHandle.h"
#include <cstddef>
typedef char *NodeData_t;

enum TreeChildSide
{
    tcsLeft = 0,
    tcsRight = 1
};

struct Node
{
    Node *left;
    Node *right;
    NodeData_t data;

    // meta
    int _level;
};

struct Tree
{
    Node *root;
    // int (*Comp)(const void *, const void *);
    void (*NodeDataToStr)(const NodeData_t data, char *const out, const size_t outSize);
    void (*DeleteNodeData)(NodeData_t data);

    // meta
    size_t elementAmount;
    unsigned maxLevel;
};

Error NewNode(Node **nodePtr, NodeData_t data);

void AddToNode(Node *root, NodeData_t data, TreeChildSide childSide);

void DeleteNode(Node *node);

Error NewTree(Tree **treePtr, NodeData_t rootData,
              void (*nodeDataToStr)(const NodeData_t data, char *const out, const size_t outSize),
              void (*DeleteNodeData)(NodeData_t data));

void DeleteTree(Tree *tree);

void DeleteAllNodes(Node *root, void (*DeleteNodeData)(NodeData_t));

// Error AddToTree(Tree *const tree, const NodeData_t data);

Node *FindInTree(Tree *const tree, const NodeData_t dataToSearch, Node **prevOfFoundNode);

Error DumpNodes(const Node *node, char *const out, char **curPtr, const size_t outSize,
                void (*nodeDataToStr)(const NodeData_t data, char *out, const size_t outSize));

#endif
