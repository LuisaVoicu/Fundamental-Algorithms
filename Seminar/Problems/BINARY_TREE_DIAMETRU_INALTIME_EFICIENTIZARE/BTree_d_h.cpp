#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int h;
    int d;
}HD;

typedef struct tree
{
    HD hd;
    int key;
    struct tree* right;
    struct tree* left;
}tree;




HD makeHD(int h, int d)
{
    HD x;
    x.h = h;
    x.d = d;
    return x;
}
tree* createNode(int key)
{
    tree* node = (tree*)malloc(sizeof(tree));
    node->key = key;
    node->left = node->right = NULL;
    node->hd = makeHD(0, 0);
    return node;
}




tree* insertBT(tree* root, int key)
{
    if (root == NULL)
    {
        return createNode(key);
    }
    if (key < root->key)
    {
        root->left = insertBT(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = insertBT(root->right, key);
    }
    return root;
}



void preOrder(tree* t)
{
    if (t == NULL)
        return;
    printf("%d ", t->key);
    preOrder(t->left);
    preOrder(t->right);
}



int max(int a, int b)
{
    int aux = a;
    if (aux < b)
        aux = b;
    return aux;
}

HD diametru(tree* t)
{

    if (t == NULL  )
    {
        return makeHD(-1, -1);
    }

    else {
        printf("%d ", t->key);
        HD L, R;
        L = diametru(t->left);
        R = diametru(t->right);
        t->hd.h = max(L.h, R.h) + 1;
        t->hd.d = max(L.h + R.h + 2, max(R.d, L.d));
        return  t->hd;
    }
    return makeHD(0, 0);


}

int main()
{

    tree* root = NULL;

    int keys[] = { 15,25,20,18,17,29,30,31 };
    root = insertBT(root, keys[0]);
    for (int i = 1; i < sizeof(keys) / sizeof(int); i++)
        insertBT(root, keys[i]);

    HD x = diametru(root);
    printf("\n%d %d\n", x.d, x.h);
    //preOrder(root);
 
}