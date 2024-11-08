#include "Tree.h"

#include <malloc.h>
#include "ColorPrint.h"


static void ErrPlaceCtor           (TreeErrorType* Err, const char* File, int Line, const char* Func);
static void PrintPlace             (const char* File, int Line, const char* Function);
static void PrintError             (const TreeErrorType* Err);
static TreeErrorType TreeVerif     (const Node_t* node, TreeErrorType* Err, const char* File, int Line, const char* Func);
static TreeErrorType TreeDtorHelper(Node_t* node, TreeErrorType* Err);




TreeErrorType NodeCtor(Node_t** node, TreeElem_t value, Node_t* left, Node_t* right)
{
    TreeErrorType Err = {};

    *node = (Node_t*) calloc(1, sizeof(Node_t));
    printf("aaaaa\n");
    if (*node == NULL)
    {
        Err.IsFatalError   = 1;
        Err.CtorCallocNull = 1;
        return TREE_VERIF(*node, Err);
    }

    (*node)->data  = value;

    (*node)->left  = left;
    (*node)->right = right;

    return TREE_VERIF(*node, Err);
}


TreeErrorType NodeDtor(Node_t* node)
{
    TreeErrorType Err = {};
    // printf("in dtor node addr = %p\n", node);

    if (node->left || node->right)
    {
        Err.IsFatalError            = 1;
        Err.DtorNodeThatChildrenHas = 1;
        return TREE_VERIF(node, Err);
    }

    node->data  = 0;
    node->left  = NULL;
    node->right = NULL;
    free(node);
    
    return TREE_VERIF(NULL, Err);
}

TreeErrorType TreeCtor(Tree_t* tree, Node_t* root, size_t treeSize = 1)
{
    TreeErrorType Err = {};

    if (root)
    {
        tree->root = root;
        tree->size = treeSize;
        return Err;
    }

    Node_t* new_root = NULL;

    tree->root = new_root;
    tree->size = 0;

    return Err;
}


TreeErrorType TreeDtor(Tree_t* tree)
{
    TreeErrorType Err = {};

    TreeDtorHelper(tree->root, &Err);

    // printf("in dtor root = %p\n", tree->root);
    tree->size = 0;
    tree->root = NULL;
    return TREE_VERIF(tree->root, Err);
}


static TreeErrorType TreeDtorHelper(Node_t* node, TreeErrorType* Err)
{
    if (node == NULL)
    {
        return *Err;
    }

    TreeDtorHelper(node->left,  Err);
    TreeDtorHelper(node->right, Err);

    NodeDtor(node);

    return *Err;
}

TreeErrorType TreeInsertInt(Tree_t* tree, TreeElem_t value)
{
    TreeErrorType Err = {};
    tree->size++;

    Node_t* node = tree->root;
    while (1)
    {
        if (node->left && node->data >= value)
        {
            node = node->left;
            continue;
        }

        if (node->right && node->data < value)
        {
            node = node->right;
            continue;
        }

        break;
    }

    // printf("left = %p\nright = %p\n", node->left, node->right);

    if (node->left == NULL && node->data >= value)
    {
        NodeCtor(&node->left, value, NULL, NULL);
        return TREE_VERIF(node->left, Err);
    }

    if (node->right == NULL && node->data < value)
    {
        NodeCtor(&node->right, value, NULL, NULL);
        return TREE_VERIF(node->right, Err);
    }

    Err.IsFatalError             = 1;
    Err.InsertIncorrectSituation = 1;
    return TREE_VERIF(node, Err);
}

TreeErrorType TreeVerif(const Node_t* node, TreeErrorType* Err, const char* File, int Line, const char* Func)
{
    ErrPlaceCtor(Err, File, Line, Func);

    if (node == NULL)
    {
        return *Err;
    }

    return *Err;
}


void PrintTree(const Node_t* node)
{

    if (node == NULL)
    {
        // COLOR_PRINT(RED, "WTF?!\n");        
        return;
    }
    
    printf("( ");

    printf("%d ", node->data);

    if (node->left)
    {
        PrintTree(node->left);
    }

    else
    {
        printf("( * ) ");
    }

    if (node->right)
    {
        PrintTree(node->right);
    }

    else
    {
        printf("( * ) ");
    }


    printf(") ");
    return;
}



static void PrintError(const TreeErrorType* Err)
{
    if (Err->IsFatalError == 0)
    {
        return;
    }

    if (Err->CtorCallocNull == 1)
    {
        COLOR_PRINT(RED, "Error: failed alocate memory in ctor.\n");
    }

    if (Err->InsertIncorrectSituation == 1)
    {
        COLOR_PRINT(RED, "Error: undefined situation in insert.\n");
    }

    if (Err->DtorNodeThatChildrenHas)
    {
        COLOR_PRINT(RED, "Error: Dtor node that childern has.\n");
    }

    return;
}

static void PrintPlace(const char* File, int Line, const char* Function)
{
    COLOR_PRINT(WHITE, "File [%s]\nLine [%d]\nFunc [%s]\n", File, Line, Function);
    return;
}

//----------------------------------------------------------------------------------------------------------------------

void TreeAssertPrint(TreeErrorType* Err, const char* File, int Line, const char* Func)
{
    if (Err->IsFatalError == 1) 
    {
        COLOR_PRINT(RED, "\nAssert made in:\n");
        PrintPlace(File, Line, Func);
        PrintError(Err);
        PrintPlace(Err->Place.File, Err->Place.Line, Err->Place.Func);
        printf("\n");
    }
    return;
}

//----------------------------------------------------------------------------------------------------------------------

static void ErrPlaceCtor(TreeErrorType* Err, const char* File, int Line, const char* Func)
{
    Err->Place.File = File;
    Err->Place.Line = Line;
    Err->Place.Func = Func;
    return;
}

//----------------------------------------------------------------------------------------------------------------------
