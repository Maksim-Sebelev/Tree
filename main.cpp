#include "Tree.h"
#include <assert.h>


int main()
{
    Node_t* n1 = {};
    Tree_t tree = {};
    // TREE_ASSERT(NodeCtor(&n1, 50, NULL, NULL));
    // TREE_ASSERT(TreeCtor  (&tree, n1, 1));
    // TREE_ASSERT(TreeInsertInt(&tree, 30));
    NodeCtor(&n1, 50, NULL, NULL);
    TreeCtor(&tree, n1, 1);
    TreeInsertInt(&tree, 30);
    TREE_ASSERT(TreeInsertInt(&tree, 70));
    TREE_ASSERT(TreeInsertInt(&tree, 10));
    TREE_ASSERT(TreeInsertInt(&tree, 80));
    TREE_ASSERT(TreeInsertInt(&tree, 65));
    TREE_ASSERT(TreeInsertInt(&tree, 66));

    // PrintPrefTree(n1);


    printf("n1.data = %d\n", n1->data);
    printf("n2.data = %d\n", n1->left->data);
    assert(tree.root);
    assert(tree.root->left);

    PrintPrefTree(&tree);
    PrintPostTree(&tree);
    PrintInfixTree(&tree);
    PrintSortTree(&tree);
    printf("\n");
    TREE_ASSERT(TreeDtor(&tree));

    return 0;
}
