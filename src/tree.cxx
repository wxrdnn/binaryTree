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

void DeleteNode(Node *node)
{
    ASSERT(node);
    free(node);
    return;
}

Error NewTree(Tree **treePtr, NodeData_t rootData, int (*comp)(const void *, const void *),
              void (*nodeDataToStr)(const NodeData_t data, char *const out, const size_t outSize))
{
    ASSERT(treePtr);
    ASSERT(comp);
    ASSERT(nodeDataToStr);

    *treePtr = (Tree *)calloc(1, sizeof(Tree));
    ALLOCATION_CHECK(treePtr);

    RETURN_ERROR_IF_FAIL(NewNode(&(*treePtr)->root, rootData));
    (*treePtr)->root->_level = 0;

    (*treePtr)->comp = comp;
    (*treePtr)->nodeDataToStr = nodeDataToStr;
    (*treePtr)->elementAmount = 1;
    (*treePtr)->maxLevel = 0;

    return cgSuccessErrorStruct;
}

Error AddToTree(Tree *const tree, const NodeData_t data)
{
    ASSERT(tree);

    Node *prevOfFoundNode = NULL;
    Node *foundNode = FindInTree(tree, data, &prevOfFoundNode);

    if (foundNode)
    {
        char buf[cMaxErrorContextLen] = {};
        tree->nodeDataToStr(foundNode->data, buf, cMaxErrorContextLen);
        return CreateError(ecElementAlreadyExists, buf);
    }

    int compRes = tree->comp(&data, &prevOfFoundNode->data);

    // fprintf(stderr, "DEBUG: AddToTree(): compRes = %d\n", compRes);

    RETURN_ERROR_IF_FAIL(NewNode(compRes > 0 ? &prevOfFoundNode->right : &prevOfFoundNode->left, data));

    return cgSuccessErrorStruct;
}

void DeleteTree(Tree *tree)
{
    ASSERT(tree);

    DeleteAllNodes(tree->root);
    free(tree);
}

void DeleteAllNodes(Node *root)
{
    if (!root)
    {
        return;
    }
    DeleteAllNodes(root->left);
    DeleteAllNodes(root->right);

    DeleteNode(root);
    return;
}

Node *FindInTree(Tree *const tree, const NodeData_t dataToSearch, Node **prevOfFoundNode)
{
    ASSERT(tree);
    ASSERT(tree->root);
    ASSERT(prevOfFoundNode);

    *prevOfFoundNode = tree->root;
    Node *curNode = tree->root;

    do
    {
        int compRes = tree->comp(&dataToSearch, &curNode->data);

        if (compRes > 0)
        {
            *prevOfFoundNode = curNode;
            curNode = (*prevOfFoundNode)->right;
        }
        else if (compRes < 0)
        {
            *prevOfFoundNode = curNode;
            curNode = (*prevOfFoundNode)->left;
        }
        else
        {
            return curNode;
        }
    } while (curNode != NULL);

    return curNode;
}

Error DumpNodes(const Node *node, char *const out, char **curPtr, const size_t outSize,
                void (*nodeDataToStr)(const NodeData_t data, char *out, const size_t outSize))
{
    ASSERT(node);
    ASSERT(out);
    ASSERT(curPtr);
    ASSERT(*curPtr);
    ASSERT(nodeDataToStr);

    **curPtr = '(';
    ++*curPtr;

    nodeDataToStr(node->data, *curPtr, outSize - (*curPtr - out));
    *curPtr += strlen(*curPtr);

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
        **curPtr = ')';
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
        **curPtr = ')';
        ++*curPtr;
        **curPtr = '\0';
    }

    **curPtr = ')';
    ++*curPtr;
    **curPtr = '\0';
    return cgSuccessErrorStruct;
}
