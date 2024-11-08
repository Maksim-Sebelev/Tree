#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include "ColorPrint.h"

typedef int TreeElem_t;


struct ErrPlace
{
    const char* File;
    int         Line;
    const char* Func;
};


struct TreeErrorType
{
    unsigned char IsFatalError              : 1;
    unsigned char CtorCallocNull            : 1;
    unsigned char InsertIncorrectSituation  : 1;
    unsigned char DtorNodeThatChildrenHas   : 1;

    ErrPlace      Place;
};


struct Node_t
{
    TreeElem_t data;
    Node_t*    right;
    Node_t*    left;
};

struct Tree_t
{
    Node_t* root;
    size_t  size;
};


TreeErrorType NodeCtor      (Node_t** node, TreeElem_t value, Node_t* left, Node_t* right);
TreeErrorType NodeDtor      (Node_t* node);
TreeErrorType TreeCtor      (Tree_t* tree, Node_t* root, size_t treeSize);
TreeErrorType TreeDtor      (Tree_t* root);
TreeErrorType TreeInsertInt    (Tree_t* node, TreeElem_t value);

void PrintTree(const Node_t* node);


#define TREE_VERIF(TreePtr, Err) TreeVerif(TreePtr, &Err, __FILE__, __LINE__, __func__)


#define TREE_RETURN_IF_ERR(TreePtr, Err) do                          \
{                                                                     \
    TreeErrorType ErrCopy = Err;                                       \
    Verif(TreePtr, &ErrCopy, __FILE__, __LINE__, __func__);             \
    if (ErrCopy.IsFatalError == 1)                                       \
    {                                                                     \
        return ErrCopy;                                                    \
    }                                                                       \
} while (0)                                                                  \



#define TREE_ASSERT(Err) do                                \
{                                                           \
    TreeErrorType ErrCopy = Err;                             \
    TreeAssertPrint(&ErrCopy, __FILE__, __LINE__, __func__);  \
    if (ErrCopy.IsFatalError == 1)                             \
    {                                                           \
        COLOR_PRINT(CYAN, "abort() in 3, 2, 1...\n");            \
        abort();                                                  \
    }                                                              \
} while (0)                                                         \

void TreeAssertPrint(TreeErrorType* Err, const char* File, int Line, const char* Func);

#endif
