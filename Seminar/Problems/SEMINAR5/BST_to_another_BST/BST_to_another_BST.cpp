#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct BType
{
    int key;
    BType* left;
    BType* right;
    BType* parent;
}BType;

BType* makeNode(int key)
{
    BType* node = (BType*)malloc(sizeof(BType));
    node->key = key;
    node->left = node->right = node->parent = NULL;
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
        printf("%d ", tree->key);
        preorder(tree->left);
        preorder(tree->right);
    }
}


BType* RightRotate(BType* tree)
{
    BType* rotTree = tree->left;
     rotTree->left = tree->left->left;
     tree->left = tree->left->right;
     rotTree->right = tree;
    return rotTree;
}

BType* LeftRotate(BType* tree)
{
    BType* rotTree = tree->right;
    rotTree->right = tree->right->right;
    tree->right = tree->right->left;
    rotTree->left = tree;
    return rotTree;
}

BType* Transform(BType* tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    if (tree->left == NULL)
    {
        tree->right = Transform(tree->right);
        return tree;
    }
    else
    {
        return Transform(RightRotate(tree));
    }
}



int SearchKeyInBST(BType* tree, int key)
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
        else if (key == tree->key)
        {
            return 1;
        }
    }
}

int HasAllKeys(BType* T1, BType* T2)
{
    while (T1 != NULL)
    {
        if (!SearchKeyInBST(T2, T1->key))
        {
            return 0;
        }
        T1 = T1->right;
    }
    return 1;
}


void TransformToAnotherBST(BType** T1, BType* T2)
{
   
    if (T2 != NULL)
    {
        
        while ((*T1)->key < T2->key)
        {
            *T1 = LeftRotate(*T1);
        }
         while ((*T1)->key > T2->key)
        {
            *T1 = RightRotate(*T1);
        }
        TransformToAnotherBST(&((*T1)->left), T2->left);
        TransformToAnotherBST(&((*T1)->right), T2->right);
    }
}

BType* TransformToAnotherBSTInit(BType* T1, BType* T2)
{
    T1 = Transform(T1);
    // verificam daca T2 are aceleasi chei ca si T1 ~~ n * h pasi (Aparent!) --> dar pentru unele chei ne vom opri mai devreme => in total n pasi
    int valid = HasAllKeys(T1, T2);
    if (!valid)
    {
        printf("Date Invalide");
        exit(1234);
    }
    TransformToAnotherBST(&T1, T2);
    return T1;
}

int main()
{
    int T1[] = { 5, 7, 3,1,4,2, 13,9,11 };
   // int T2[] = { 1, 3, 2, 5, 4, 7, 13, 9, 11 };
    //int T2[] =   { 1, 7, 3, 2, 5, 4, 9, 13, 11 };
    int T2[] = { 7, 1 , 3, 2, 5, 4, 9, 13, 11 };
    //int T2[] = { 4,3,2,1,5,7,9,11, 13};

    BType* tree = NULL;
    BType* tree2 = NULL;
    tree = insertBT(tree, T1[0]);
    tree2 = insertBT(tree2, T2[0]);
    for (int i = 1; i < sizeof(T1) / sizeof(int); i++)
    {
        insertBT(tree, T1[i]);
        insertBT(tree2, T2[i]);
    }



    tree = TransformToAnotherBSTInit(tree, tree2);

    printf("\n\n");
    preorder(tree);
    printf("\n");
    preorder(tree2);

}