#include <stdio.h>
#include <stdlib.h>

typedef struct Type
{
    int key;
    Type* prev_left;
    Type* next_right;
    Type* parent;
}Type;


Type* makeNode(int key)
{
    Type* node = (Type*)malloc(sizeof(Type));
    node->key = key;
    node->parent = node->prev_left = node->next_right = NULL;
    return node;
}


Type* insertBT(Type* tree, int key)
{
    if (tree == NULL){
        return makeNode(key);
    }
    if (key < tree->key)
    {
        tree->prev_left = insertBT(tree->prev_left, key);
        if (tree->prev_left != NULL)
        {
            tree->prev_left->parent = tree;
        }
    } else{
        tree->next_right = insertBT(tree->next_right, key);
        if (tree->next_right != NULL)
        {
            tree->next_right->parent = tree;
        }
    }
    return tree;
}

void inorder(Type* tree)
{
    Type* aux0 = NULL;
    Type* aux1 = NULL;
    Type* aux2 = NULL;


    if (tree != NULL)
    {
        inorder(tree->prev_left);
        //printf("%d\n", tree->key);
        /*if (tree->prev_left == NULL)
        {
            aux1 = tree;
            if (aux0 == NULL)
                aux0 = tree;
            else
            {
                aux0->next_right = tree;
                aux0->next_right->prev_left = aux0;
                aux0 = aux0->next_right;
            }
            printf("x%d\n", aux1->key);
        }
        if (tree->next_right != NULL&&tree->prev_left!=NULL)
        {
            printf("xxx%d\n", tree->key);
        }
        */
        inorder(tree->next_right);
        printf("%d ", tree->key);

    /*
        if (tree->next_right == NULL && tree->prev_left!=NULL)
        {
            aux2 = tree;
            printf("x%d\n", aux2->key);
        }*/
    }
}

Type* postorder(Type* tree)
{
    
    Type* aux =NULL;
    Type* fromLeft = NULL;
    Type* fromRight = NULL;
    if (tree != NULL)
    {

        fromLeft = postorder(tree->prev_left);
        fromRight = postorder(tree->next_right);


        printf("before: ");
        if (fromLeft != NULL)
        {
            printf(" lf %d   ", fromLeft->key);
            if (fromLeft->next_right != NULL)
            {
                printf(" next: %d /// ", fromLeft->next_right->key);
            }
        }
        if (fromRight != NULL)
        {
            printf("lR %d", fromRight->key);
        }
        printf("---\n\n");


        Type* aux = tree;
      //  printf("nod %d  \n", aux->key);
   

        

       /* if (tree->prev_left != NULL)
        {
            printf("%d ", tree->prev_left->key);
        }
        else printf("x ");
 
        if (tree->next_right != NULL)
        {
            printf("%d\n", tree->next_right->key);

        }
        else printf("x\n");*/



        if (fromLeft == NULL)
        {
            fromLeft = aux;

            if(fromRight == NULL)
            { 
                fromRight = fromLeft;
            }
            else
            {
                fromLeft->next_right = fromRight;
                fromRight->prev_left = fromLeft;
            }
        /*    if (aux->prev_left != NULL)
            {
                aux->prev_left->next_right = fromLeft;
            }*/


        }
        else
        {

            Type* res = fromLeft;
            while (res->next_right != NULL)
            {
                printf("%d ", res->key);
                res = res->next_right;
            }

            if (fromRight == NULL) 
            {
                fromRight = aux;
                res->next_right = fromRight;
            }
            else
            {
                aux->next_right = fromRight;
                fromRight->prev_left = aux;

           
                res->next_right = aux;
                aux->prev_left = res;
            }

            
        }
        /*
        if (fromRight == NULL)
        {
            fromRight = aux;
            if (aux->prev_left != NULL)
            {
                aux->prev_left->next_right = fromRight;
            }
            printf("-------> %d\n", fromRight->key);
        }
        else
        {
            printf("-------> %d %d\n", aux->key, fromRight->key);
            aux->next_right = fromRight;
            fromRight->prev_left = aux;
        }
        */
  /*      if(fromLeft->prev_left!=NULL)

        printf("lfp %d   ", fromLeft->prev_left->key);*/

        printf("lf %d   ", fromLeft->key);
            if (fromLeft->next_right != NULL)
            {
                printf(" next: %d /// ", fromLeft->next_right->key);
            }
        if (fromRight != NULL)
        {
            printf("lR %d", fromRight->key);
        }
        printf("\n");

        printf("\n_______%d________\n",aux->key);
        Type* res = fromLeft;
        while (res != NULL)
        {
            printf("%d ", res->key);
            res = res->next_right;
        }
        printf("\n_______________\n");

        
     
        //fromLeft = fromLeft->next_right;
        return fromLeft;

    }
    else return NULL;
}

void post(Type* tree)
{
    if (tree != NULL)
    {
        post(tree->prev_left);
        post(tree->next_right);
        printf("%d ", tree->key);
    }
}

enum DIRECTION
{
    ABOVE,
    LEFT,
    RIGHT
};



void iterativeInorder(Type* tree)
{
    DIRECTION direction = ABOVE;
    
    Type* aux;

    while (tree != NULL)
    {
        if (direction == ABOVE)
        {
            if (tree->prev_left != NULL)
            {
                tree = tree->prev_left;
            }
            else
            {
                direction = LEFT;
            }
        }
        else if (direction == LEFT)
        {
            printf("%d ", tree->key);

       
            if (tree->next_right != NULL)
            {
                //printf(" urmeaza: %d\n", tree->next_right->key);
                
                tree = tree->next_right;
                direction = ABOVE;
            }
            else
            {
                direction = RIGHT;
            }


        }
        else
        {
            if (tree->parent != NULL && tree == tree->parent->prev_left)
            {
                direction = LEFT;
            }
            tree = tree->parent;
        }
    }
}



Type* leftmost(Type* tree)
{
    if (tree == NULL)
        return NULL;
    if (tree->prev_left == NULL)
        return tree;
    leftmost(tree->prev_left);
}


Type* rightmost(Type* tree)
{
    if (tree == NULL)
        return NULL;
    if (tree->next_right == NULL)
        return tree;
    leftmost(tree->next_right);
}



Type* BSTtoDLL(Type* root)
{
    if (root == NULL)
        return NULL;

    Type* fromRight = BSTtoDLL(root->next_right);
    Type* fromLeft = BSTtoDLL(root->prev_left);

    if (fromLeft != NULL)
    {
        Type* auxr = rightmost(fromLeft);
        auxr->next_right = root;
        root->prev_left = auxr;
    }

    if (fromRight != NULL) {

        Type* auxl = leftmost(fromRight);
        if (auxl != NULL)
        {
            root->next_right = auxl;
            auxl->prev_left = root;

        }

        return rightmost(fromRight);
    }
    else 
    {
        return root;
    }

}



int main()
{

    //int a[] = { 5,1,0,4,3,2,7,6,10,21 };
    //int a[] = { 8,3,6,4,7,1,10,14,13 };
    //int a[] = { 1,2,4,5,6 };
    int a[] = { 5,4,3,2,1 };
    int n = sizeof(a) / sizeof(int);
    Type* root = NULL;
    root = insertBT(root, a[0]);
    for (int i = 1; i < n; i++)
    {
        insertBT(root, a[i]);
    }


    Type* res = NULL;


    printf("\n");
    iterativeInorder(root);
    printf("\n");

    /*
    res = postorder(root);
    printf("\n\n");
    while (res != NULL)
    {
        printf("%d ", res->key);
        res = res->next_right;
    }
    */

    res = BSTtoDLL(root);
    printf("\n\n");
    while (res != NULL)
    {
        printf("%d ", res->key);
        res = res->prev_left;
    }
}