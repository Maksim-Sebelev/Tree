#include "Tree.h"

#include <malloc.h>
#include <assert.h>
#include "ColorPrint.h"


static void ErrPlaceCtor               (TreeErrorType* Err, const char* File, int Line, const char* Func);
static void PrintPlace                 (const char* File, int Line, const char* Function);
static void PrintError                 (const TreeErrorType* Err);

static TreeErrorType TreeVerif         (const Node_t* node, TreeErrorType* Err, const char* File, int Line, const char* Func);
static TreeErrorType TreeDtorHelper    (Node_t* node, TreeErrorType* Err);

static void PrintPrefTreeHelper        (const Node_t* node);
static void PrintPostTreeHelper        (const Node_t* node);
static void PrintInfixTreeHelper       (const Node_t* node);
static void PrintSortTreeHelper        (const Node_t* node);




//----------------------------------------------------------------------------------------------------------------------

TreeErrorType NodeCtor(Node_t** node, TreeElem_t value, Node_t* left, Node_t* right)
{
    TreeErrorType Err = {};

    *node = (Node_t*) calloc(1, sizeof(Node_t));

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

//----------------------------------------------------------------------------------------------------------------------

TreeErrorType NodeDtor(Node_t* node)
{
    TreeErrorType Err = {};

    // if (node->left || node->right)
    // {
    //     COLOR_PRINT(RED, "DTOR ERR.\n");
    //     Err.IsFatalError            = 1;
    //     Err.DtorNodeThatChildrenHas = 1;
    //     return TREE_VERIF(node, Err);
    // }


    node->data  = 0;
    node->left  = NULL;
    node->right = NULL;
    free(node);
    
    return TREE_VERIF(NULL, Err);
}

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

TreeErrorType TreeDtor(Tree_t* tree)
{
    assert(tree);
    assert(tree->root);

    TreeErrorType Err = {};

    TreeDtorHelper(tree->root, &Err);

    tree->size = 0;
    tree->root = NULL;
    return TREE_VERIF(tree->root, Err);
}

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

TreeErrorType TreeInsertInt(Tree_t* tree, TreeElem_t value)
{
    assert(tree);
    assert(tree->root);

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

//----------------------------------------------------------------------------------------------------------------------

void PrintPrefTree(const Tree_t* tree)
{
    assert(tree);
    COLOR_PRINT(GREEN, "Pref tree:\n");
    PrintPrefTreeHelper(tree->root);
    printf("\n");
    return;
}

//----------------------------------------------------------------------------------------------------------------------

static void PrintPrefTreeHelper(const Node_t* node)
{
    if (node == NULL)
    {
        return;
    }
    
    printf("( ");

    printf("%d ", node->data);

    if (node->left)
    {
        PrintPrefTreeHelper(node->left);
    }

    else
    {
        printf("(*) ");
    }


    if (node->right)
    {
        PrintPrefTreeHelper(node->right);
    }

    else
    {
        printf("(*) ");
    }


    printf(") ");

    return;
}

//----------------------------------------------------------------------------------------------------------------------

void PrintPostTree(const Tree_t* tree)
{
    COLOR_PRINT(GREEN, "Post tree:\n");
    PrintPostTreeHelper(tree->root);
    printf("\n");
    return;
}

//----------------------------------------------------------------------------------------------------------------------

static void PrintPostTreeHelper(const Node_t* node)
{
    if (node == NULL)
    {
        return;
    }

    printf("( ");

    if (node->left)
    {
        PrintPostTreeHelper(node->left);
    }

    else
    {
        printf("( * ) ");
    }

    if (node->right)
    {
        PrintPostTreeHelper(node->right);
    }

    else
    {
        printf("( * ) ");
    }

    printf("%d ", node->data);

    printf(") ");

    return;
}

//----------------------------------------------------------------------------------------------------------------------

void PrintInfixTree(const Tree_t* tree)
{
    COLOR_PRINT(GREEN, "Infix tree:\n");
    PrintInfixTreeHelper(tree->root);
    printf("\n");
    return;
}

//----------------------------------------------------------------------------------------------------------------------

static void PrintInfixTreeHelper(const Node_t* node)
{
    if (node == NULL)
    {
        return;
    }

    printf("( ");

    if (node->left)
    {
        PrintInfixTreeHelper(node->left);
    }

    else
    {
        printf("(*) ");
    }

    printf("%d ", node->data);

    if (node->right)
    {
        PrintInfixTreeHelper(node->right);   
    }

    else
    {
        printf("(*) ");
    }



    printf(") ");
    
    return;
}

//----------------------------------------------------------------------------------------------------------------------

void PrintSortTree(const Tree_t* tree)
{
    COLOR_PRINT(GREEN, "Sort tree:\n");
    PrintSortTreeHelper(tree->root);
    printf("\n");
    return;
}

//----------------------------------------------------------------------------------------------------------------------

static void PrintSortTreeHelper(const Node_t* node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->left)
    {
        PrintSortTreeHelper(node->left);
    }

    printf("%d ", node->data);

    if (node->right)
    {
        PrintSortTreeHelper(node->right);
    }

    return;
}


//----------------------------------------------------------------------------------------------------------------------

static void PrintError(const TreeErrorType* Err)
{
    if (Err->IsFatalError == 0)
    {
        COLOR_PRINT(RED, "print err no err.\n");        
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

    if (Err->DtorNodeThatChildrenHas == 1)
    {
        COLOR_PRINT(RED, "Error: Dtor node that childern has.\n");
    }

    return;
}

//----------------------------------------------------------------------------------------------------------------------

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
        COLOR_PRINT(RED,    "\nAssert made in:\n");
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
