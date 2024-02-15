#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _BType
{
    int key;
    int dim;
    _BType* right;
    _BType* left;
    _BType* parent;
}BType;


BType* makeNode(int key)
{
    BType* node = (BType*)malloc(sizeof(BType));
    node->key = key;
    node->dim = 0;
    node->left = node->right = node->parent =  NULL;
    return node;
}

BType* insertBT(BType* tree, int key)
{
    if (tree == NULL)
    {
        return makeNode(key);
    }
    if (key < tree->key)
    {
        tree->left = insertBT(tree->left, key);
        if (tree->left != NULL)
        {
            tree->left->parent = tree;
        }
    }
    else
    {
        tree->right = insertBT(tree->right, key);
        if (tree->right != NULL)
        {
            tree->right->parent = tree;
        }
    }

    return tree;
}


void preorder(BType* tree)
{
    if (tree != NULL)
    {
        printf("%d %d\n", tree->key, tree->dim);
        preorder(tree->left);
        preorder(tree->right);
    }
}



BType* searchByKey(BType* tree, int key)
{
    while (tree != NULL)
    {
        if (key < tree->key)
        {
            tree = tree->left;
        }
        else if (key > tree->key)
        {
            tree = tree->right;
        }
        else
        {
            return tree;
        }
    }
}

void getDimensions(BType* tree)
{
    if (tree == NULL)
    {
        return;
    }

    getDimensions(tree->left);
    getDimensions(tree->right);

    if (tree->left != NULL)
    {
        tree->dim += tree->left->dim;
    }
    if (tree->right != NULL)
    {
        tree->dim += tree->right->dim;
    }
    tree->dim += 1;
}



BType* OS_Select(BType* T, int i)
{
    int r = 1;
    if (T == NULL)
    {
        return NULL;
    }

    if (T->left != NULL)
    {
        r += T->left->dim;
    }

    if (i == r)
    {
        return T;
    }
    else if (i < r)
    {
        return OS_Select(T->left, i);
    }
    else
    {
        return OS_Select(T->right, i - r);
    }
}


int OS_Rank(BType* T, BType* x)
{
    int r = 1;
    if (x->left != NULL)
    {
        r += x->left->dim; // cate elemente sunt in stanga
    }
    BType* y = x;
    while (y->parent != NULL)
    {
        if (y == y->parent->right)
        {
            r += 1;
            if (y->parent->left != NULL)
            {
                r += y->parent->left->dim;
            }
        }
        y = y->parent;
    }

    return r;

}

BType* KSuccessor(BType* T, BType* x, int k)
{
    return OS_Select(T, OS_Rank(T, x) + k);
}

int main()
{
    int T1[] = { 5, 7, 3,1,4,2, 13,9,11 };


    BType* tree = NULL;
    tree = insertBT(tree, T1[0]);
    for (int i = 1; i < sizeof(T1) / sizeof(int); i++)
    {
        insertBT(tree, T1[i]);
    }

    getDimensions(tree);
    preorder(tree);



    BType* result = KSuccessor(tree, searchByKey(tree, 7),3);
    if (result != NULL) {
        printf("-->%d\n", result->key);
    }
    else printf("depaseste");
}