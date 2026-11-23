#include "h/tree.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

Error NewNode(Node **nodePtr, NodeData_t data)
{
    ASSERT(nodePtr);

    *nodePtr = (Node *)calloc(1, sizeof(Node));
    ALLOCATION_CHECK(*nodePtr);

    (*nodePtr)->left = NULL;
    (*nodePtr)->right = NULL;

    (*nodePtr)->data = data;
    (*nodePtr)->_level = -1;

    return cgSuccessErrorStruct;
}

void AddToNode(Node *root, NodeData_t data, TreeChildSide childSide)
{
    ASSERT(root);

    switch (childSide)
    {
    case tcsLeft:
        NewNode(&root->left, data);
        break;

    case tcsRight:
        NewNode(&root->right, data);
        break;
    }

    return;
}

void DeleteNode(Node *node)
{
    ASSERT(node);
    free(node);
    return;
}

Error NewTree(Tree **treePtr, NodeData_t rootData,
              void (*nodeDataToStr)(const NodeData_t data, char *const out, const size_t outSize),
              void (*DeleteNodeData)(NodeData_t data))
{
    ASSERT(treePtr);
    ASSERT(nodeDataToStr);

    *treePtr = (Tree *)calloc(1, sizeof(Tree));
    ALLOCATION_CHECK(treePtr);

    RETURN_ERROR_IF_FAIL(NewNode(&(*treePtr)->root, rootData));
    (*treePtr)->root->_level = 0;

    // (*treePtr)->Comp = comp;
    (*treePtr)->NodeDataToStr = nodeDataToStr;
    (*treePtr)->DeleteNodeData = DeleteNodeData;
    (*treePtr)->elementAmount = 1;
    (*treePtr)->maxLevel = 0;

    return cgSuccessErrorStruct;
}

// Error AddToTree(Tree *const tree, const NodeData_t data, TreeChildSide childSide)
// {
//     ASSERT(tree);
//
//     Node *prevOfFoundNode = NULL;
//     Node *foundNode = FindInTree(tree, data, &prevOfFoundNode);
//
//     if (foundNode)
//     {
//         char buf[cMaxErrorContextLen] = {};
//         tree->NodeDataToStr(foundNode->data, buf, cMaxErrorContextLen);
//         return CreateError(ecElementAlreadyExists, buf);
//     }
//
//     int compRes = tree->Comp(&data, &prevOfFoundNode->data);
//
//     // fprintf(stderr, "DEBUG: AddToTree(): compRes = %d\n", compRes);
//
//     RETURN_ERROR_IF_FAIL(NewNode(compRes > 0 ? &prevOfFoundNode->right : &prevOfFoundNode->left, data));
//
//     return cgSuccessErrorStruct;
// }

void DeleteTree(Tree *tree)
{
    ASSERT(tree);

    DeleteAllNodes(tree->root, tree->DeleteNodeData);
    free(tree);
}

void DeleteAllNodes(Node *root, void (*DeleteNodeData)(NodeData_t))
{
    ASSERT(DeleteNodeData);

    if (!root)
    {
        return;
    }
    DeleteAllNodes(root->left, DeleteNodeData);
    DeleteAllNodes(root->right, DeleteNodeData);

    // fprintf(stderr, "DEBUG: DeleteAllNodes(): root->data=<%s>\n", root->data);

    PRINTF_DEBUG("root->data = <%s>\n", root->data);
    DeleteNodeData(root->data);
    DeleteNode(root);
    return;
}

Node *FindInTree(Node *root, const NodeData_t dataToSearch)
{
    if (root == NULL)
    {
        return root;
    }

    Node *foundInLeft = FindInTree(root->left, dataToSearch);

    if (foundInLeft == NULL)
    {
        return FindInTree(root->right, dataToSearch);
    }

    return foundInLeft;
}

Error DumpNodes(const Node *node, char *const out, char **curPtr, const size_t outSize,
                void (*nodeDataToStr)(const NodeData_t data, char *out, const size_t outSize))
{
    ASSERT(node);
    ASSERT(out);
    ASSERT(curPtr);
    ASSERT(*curPtr);
    ASSERT(nodeDataToStr);

    // fprintf(stderr, "DEBUG: DumpNodes(): out = %p; *curPtr - out = %ld\n", out, *curPtr - out);
    ASSERT(*curPtr >= out);
    ASSERT(*curPtr - out < outSize);

    **curPtr = '(';
    ++*curPtr;
    **curPtr = '|';
    ++*curPtr;

    nodeDataToStr(node->data, *curPtr, outSize - (*curPtr - out));
    *curPtr += strlen(*curPtr);
    **curPtr = '|';
    ++*curPtr;

    // fprintf(
    //     stderr, "DEBUG: DumpNodes(): node(%p)->left = %p, node(%p)->right = %p\n", node, node->left, node,
    //     node->right);

    if (node->left)
    {

        // fprintf(stderr, "DEBUG: node(%p)->left(%p)->data = %d\n", node, node->left, node->left->data);
        RETURN_ERROR_IF_FAIL(DumpNodes(node->left, out, curPtr, outSize, nodeDataToStr));
    }
    else if (node->right)
    {
        **curPtr = '(';
        ++*curPtr;
        **curPtr = '|';
        ++*curPtr;
        **curPtr = ')';
        ++*curPtr;
        **curPtr = '|';
        ++*curPtr;
        **curPtr = '\0';
    }
    if (node->right)
    {
        // fprintf(stderr, "DEBUG: node(%p)->right(%p)->data = %d\n", node, node->right, node->right->data);
        RETURN_ERROR_IF_FAIL(DumpNodes(node->right, out, curPtr, outSize, nodeDataToStr));
    }
    else if (node->left)
    {
        **curPtr = '(';
        ++*curPtr;
        **curPtr = '|';
        ++*curPtr;
        **curPtr = ')';
        ++*curPtr;
        **curPtr = '|';
        ++*curPtr;
        **curPtr = '\0';
    }

    **curPtr = ')';
    ++*curPtr;
    **curPtr = '|';
    ++*curPtr;
    **curPtr = '\0';
    return cgSuccessErrorStruct;
}
