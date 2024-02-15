/*
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
*
*  ***** DYNAMIC ORDER STATISTICS WITH RED-BLACK TREES *****
** BUILD RB-TREE WITH N NODES (*INSERT FIXUPS FOR RBT) . OS_DELETE (*DELETE FIXUPS FOR RBT) . COMPLEXITY EVALUATIONS **
*
*
*/

/*
*
***** RUNNING TIME FOR DS WITH RBT 
*** OBS: hRBT = [logN,2log(N+1)]
* 
* OS_SELECT         : O(logN)
* OS_DELETE         : O(logN) (including FixUps!) 
* OS_BUILD (INSERT) : O(logN) (inlcuding FixUps )
* 
* 
*** Rotations for fixUps:
** Delete: W: logN (doar CASE#2 - recoloring)
*         B: 1 rotation
*         A: 2/3 rotations
* 
** Insert: A: 2 rotations
*/  


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define MAX_DIM 10000
#define STEP_SIZE 100
#define NBS_TESTS 5

Profiler prof("StatisticiDinamice");

enum Color {RED,BLACK};

typedef struct RBType
{
    int key;
    int size; 
    Color color;
    RBType* parent;
    RBType* left;
    RBType* right;
}RBType;

RBType* dummyNode;




RBType* createRBNode(int key)
{
    RBType* node = (RBType*)malloc(sizeof(RBType));
    node->size = 1;

    node->key = key;
    node->color = RED;
    node->left = node->right = node->parent = dummyNode;
    return node;
}

/*
* FUNCTII AUXILIARE
*/
int getDepth(RBType* tree)
{
    if (tree == dummyNode)
        return 0;
    int counter = 1;
    while (tree->parent != dummyNode)
    {
        counter++;
        tree = tree->parent;
    }
    return counter;
}

void preorder(RBType* tree)
{
    if (tree != NULL && tree!=dummyNode)
    {
        printf("%d ", tree->key);
        printf(" %d %d ", tree->size, getDepth(tree));
        (tree->color == RED) ? printf("R\n") : printf("B\n");
        

        preorder(tree->left);
        preorder(tree->right);
    }
    
}


/* 
* INSERT / BUILD + ROTATIONS 
*/
void leftRotate(RBType** tree, RBType* node, int* countOps)
{
    
    (*countOps) += 2; // la size nu numar-- nu sunt date de intrare
    RBType* nodeRight = node->right;
    node->right = nodeRight->left;
    int szNewRight = node->left->size + nodeRight->left->size + 1;
    int szNewNode = szNewRight + nodeRight->right->size + 1;
    
    (*countOps)++;
    if (nodeRight->left != dummyNode)
    {
        (*countOps)++;
        nodeRight->left->parent = node;
    }
    (*countOps)++;
    nodeRight->parent = node->parent;
    
    
    (*countOps) += 2;
    if (node->parent == dummyNode) // e radacina
    {
        (*countOps)++;
        *tree = nodeRight;
    }
    else if (node == node->parent->left) // e copil stang
    {
        (*countOps)++;
        node->parent->left = nodeRight;
    }
    else // right child
    {
        (*countOps)++;
        node->parent->right = nodeRight;
    }

    (*countOps) += 2;
    nodeRight->left = node;
    node->parent = nodeRight;

    nodeRight->size = szNewNode;
    node->size = szNewRight;
}

void rightRotate(RBType** tree, RBType* node, int* countOps)
{

    (*countOps) += 2;
    RBType* nodeLeft = node->left;
    node->left = nodeLeft->right;
    
    int szNewLeft = nodeLeft->right->size + node->right->size + 1;
    int szNewNode = nodeLeft->left->size + szNewLeft + 1;
    
    (*countOps)++;
    if (nodeLeft->right != dummyNode)
    {
        (*countOps)++;
        nodeLeft->right->parent = node;
    }


    (*countOps)++;
    nodeLeft->parent = node->parent;

    (*countOps) += 2;
    if (node->parent == dummyNode)
    {
        (*countOps)++;
        *tree = nodeLeft;

    }
    else if (node == node->parent->left)
    {
        (*countOps)++;
        node->parent->left = nodeLeft;
    }
    else
    {
        (*countOps)++;
        node->parent->right = nodeLeft;
    }

    (*countOps) += 2;
    nodeLeft->right = node;
    node->parent = nodeLeft;

    nodeLeft->size = szNewNode;
    node->size = szNewLeft;

}

void RB_Insert_Fixup(RBType** tree, RBType* insNode, int* countOps)
{

    while ( insNode->parent->color == RED)
    {
        (*countOps)++;

        (*countOps)++;
        if (insNode->parent == insNode->parent->parent->left)
        {

            (*countOps) += 2;
            RBType* uncleRight = insNode->parent->parent->right;
            if (uncleRight->color == RED)
            {

                insNode->parent->color = BLACK;
                uncleRight->color = BLACK;
                insNode->parent->parent->color = RED;

                (*countOps)++;
                insNode = insNode->parent->parent; // DUCEM PROBLEMA MAI SUS
            }
            else
            {
                (*countOps)++;
                if (insNode == insNode->parent->right)
                {

                    // CASE #2
                    (*countOps)++;
                    insNode = insNode->parent;
                    leftRotate(tree, insNode, countOps);

                }
    
                // case #3

                insNode->parent->color = BLACK;
                insNode->parent->parent->color = RED;
                rightRotate(tree, insNode->parent->parent, countOps);
                
            }
        }
        else
        {
            // suntem in subarborele drept

            (*countOps) += 2;
            RBType* uncleLeft = insNode->parent->parent->left;
            if (uncleLeft->color == RED)
            {
                // case #1

                insNode->parent->color = BLACK;
                uncleLeft->color = BLACK;
                insNode->parent->parent->color = RED;

                (*countOps)++;
                insNode = insNode->parent->parent;
            }
            else 
            {
                (*countOps)++;
                if (insNode == insNode->parent->left)
                {

                    // case #2
                    (*countOps)++;
                    insNode = insNode->parent;
                    rightRotate(tree, insNode, countOps);

                }

                // case 3
               // printf("right 3\n");

                insNode->parent->color = BLACK;
                insNode->parent->parent->color = RED;
                leftRotate(tree, insNode->parent->parent,countOps);
            }
        }
    }

    (*tree)->color = BLACK; // reparam in cazul in care fix pe radacina punem culoarea red (exemplu case #1)
}

RBType* insertRBT(RBType** tree, int key, int* countOps)
{

    (*countOps)++;
    RBType* node = createRBNode(key);
        
    (*countOps)++;
        if (*tree == NULL)
        {
            // primul nod din RBT e BLACK
            
            (*countOps)+=2;
            node->color = BLACK;
            *tree = node;
            return *tree;
        }

        // search 

    
    (*countOps)+=2;
    RBType* aux = *tree;
    RBType* p = dummyNode;

    while (aux != dummyNode)
    {
        (*countOps) += 3;
        aux->size++;
        p = aux;
        
        (*countOps) += 2;
        if (aux->key < node->key)
        {
            aux = aux->right;
        }
        else
        {
            aux = aux->left;
        }
    }
    (*countOps)++;
    
    (*countOps)++;
    node->parent = p;
    
    (*countOps) += 2;
    if(node->key < p->key)
    {
      
        p->left = node;
    }
    else
    {
        p->right = node;
    }

    RB_Insert_Fixup(tree, node, countOps);

    
    return *tree;
}

void  buildTree(RBType** root, int left, int right, int* countOps)
{
    //printf("hello?");
    if (left > right)
    {
        return;
    }

    int middle = (right + left) / 2;

    *root = insertRBT(root, middle, countOps);

    buildTree(root, left, middle - 1, countOps);
    buildTree(root, middle + 1, right, countOps);

}




/*
* SELECT
*/

RBType* OS_Select(RBType* tree, int i, int* countOps)
{
    (*countOps)++;
    if (tree == dummyNode)
    {
        return NULL;
    }

    (*countOps)+=2;
    int nbNodesLeft = ((tree->left == NULL) ? 0 : tree->left->size) + 1;

    (*countOps)+=2;
    if (i == nbNodesLeft)
    {
        return tree;
    }
    else if (i < nbNodesLeft)
    {
        return OS_Select(tree->left, i, countOps);
    }
    else
    {
        return OS_Select(tree->right, i - nbNodesLeft, countOps);
    }


}


/*
* DELETE
*/

RBType* treeMin(RBType* root, int* countOps)
{
    (*countOps)++;
    if (root == dummyNode)
    {
        return dummyNode;
    }
    while (root->left != dummyNode)
    {
        (*countOps) += 2;
        root = root->left;
    }
    (*countOps)++;
    return root;
}

RBType* successor(RBType* tree, int* countOps)
{


    (*countOps)++;
    if (tree == dummyNode)
    {
        return dummyNode;

    }

    RBType* succ = treeMin(tree->right, countOps);

    (*countOps)++;
    if (succ != dummyNode)
    {
        return succ;
    }

    (*countOps)++;
    succ = tree;

    while (succ->parent != dummyNode && succ == succ->parent->right)
    {
        (*countOps)+=3;
        succ = succ->parent;
    }
    (*countOps) += 2;

    (*countOps)+=2;
    if (succ->parent == dummyNode || succ != succ->parent->right)
    {
        // cel mai mare element din arbore...succesorul lui e el insusi
        return tree;
    }
    return succ->parent;
}

void updateSize(RBType* tree,RBType* y, int* countOps)
{
    (*countOps)++;
    if (y == dummyNode)
        return;

    while (tree != y)
    {
        (*countOps)++;

        tree->size--;

        (*countOps) += 2;
        if (tree->key > y->key)
        {
            tree = tree->left;
        }
        else
        {
            tree = tree->right;
        }
    }
    (*countOps)++;

    y->size--;
}

RBType* deleteTree(RBType** root, RBType* z, RBType** rememberParent, int* countOps)
{

    if (*root == dummyNode)
    {
        return dummyNode;
    }
    (*countOps)++;
    RBType* y = dummyNode;

    (*countOps)+=2;
    if (z->left == dummyNode || z->left == dummyNode)
    {

        // caz 1: frunza
        (*countOps)++;
        y = z;
    }
    else
    {
        y = successor(z, countOps);
    }
    
    updateSize(*root, y, countOps);
    
    (*countOps)+=3;
    RBType* x = dummyNode;
    if (y->left != dummyNode)
    {
        x = y->left;
    }
    else 
    {
        x = y->right;
    }

    
    (*countOps)++;
    if (x != dummyNode)
    {
        (*countOps)++;
        x->parent = y->parent;
    }
    
    *rememberParent = y->parent;

    (*countOps)+=2;
    if (y->parent == dummyNode)
    {
        (*countOps)++;
        *root = x;
    }
    else if (y == y->parent->left)
    {
        (*countOps)++;
        y->parent->left = x;
    }
    else
    {
        (*countOps)++;
        y->parent->right = x;
    }
    
    (*countOps)++;
    if (y != z)
    {
        (*countOps)++;
        z->key = y->key;
        z->size = 1 + z->left->size + z->right->size;
    }   


    return y;
}

void RB_Delete_Fixup(RBType** tree, RBType* x, RBType* rememberParent, int* countOps)
{
    RBType* w; // frate x       
    RBType* parent; // frate x       

    int specialCase = 0;

    while (x != *tree && x->color == BLACK)
    {      
    
        int case1 = 0;
        int case2 = 0;
        int case3 = 0;
        int case4 = 0;
 
        (*countOps)++;
        RBType* childOfParent = x;
        if (x == dummyNode)
        {
            specialCase = 1;
            x = rememberParent; // pretindem ca nu stergem nodul initial si iteram dupa nodul deja sters
            RBType* y = rememberParent;
            childOfParent = dummyNode;
            parent = rememberParent;
        }
        else
        {
            parent = x->parent;
        }


        (*countOps)++;
        if (childOfParent == parent->left)
        {
            (*countOps)++;
            w = parent->right;


            if (w->color == RED && parent->color == BLACK)
            {
                // case #1
                case1 = 1;
                //printf("case1L\n");

                w->color = BLACK;
                parent->color = RED;
                leftRotate(tree, parent, countOps);

                (*countOps)++;
                w = parent->right;
            }
            
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                //printf("case2L\n");
                case2 = 1;
                // case #2
                w->color = RED;

                if (parent->color == RED)
                {
                    parent->color = BLACK;
                    //that's all
                    break;
                }
                else
                {
                    // double black
                (*countOps)++;
                x = parent;
                parent = x->parent;
                }

            }
            else if (case2==0 && w->left->color == RED && w->right->color == BLACK)
            {
                // case #3
                //printf("case3L\n");
                case3 = 1;

                w->left->color = BLACK;
                w->color = RED;
                rightRotate(tree, w,countOps);

                (*countOps)++;
                w = parent->right;
            }

            if (case2 == 0)
            {
            // case 4
                //printf("case4L\n");
                case4 = 1;
                w->color = parent->color;
                parent->color = BLACK;
                leftRotate(tree, parent, countOps);
                w->right->color = BLACK;

                (*countOps)+=2;
                x = *tree;
                parent = x->parent;
            }
        }
        else
        {
            (*countOps)++;
            w = parent->left;
         

            if (w->color == RED && parent->color == BLACK)
            {
                // CASE#1
                case1 = 1;
                //printf("case1R\n");
                w->color = BLACK;
                parent->color = RED;
                rightRotate(tree, parent, countOps);

                (*countOps)++;
                w = parent->left;

            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                // case #2
                case2 = 1;
                //printf("case2R\n");

                w->color = RED;
                if (parent->color == RED)
                {
                    parent->color = BLACK;
                    break;
                }
                else
                {
                    // double black
                    (*countOps)++;
                    x = parent;
                    parent = x->parent;
                }

            }
            else if (case2==0&&w->left->color == BLACK && w->right->color == RED)
            {
                 // case #3
                case3 = 1;
                //printf("case3R\n");

                w->right->color = BLACK;
                w->color = RED;
                leftRotate(tree, w, countOps);

                (*countOps)++;
                w = parent->left;
            }


            if (case2 == 0)
            {
                //printf("case4R\n");
                // case #4
                w->color = parent->color;
                parent->color = BLACK;
                rightRotate(tree, parent, countOps);
                w->left->color = BLACK;

                (*countOps)+=2;
                x = *tree;
                parent = x->parent;
            }
        }

    }
    (*countOps)++;

}

void deleteRBT(RBType** tree, RBType* z, int* countOps)
{
     RBType* rememberParent = dummyNode;
     RBType* y = deleteTree(tree, z, &rememberParent, countOps);
     
     if (y == dummyNode)
     {
         return;
     }

     RBType* x;

     if (y->left != dummyNode)
            x = y->left;
     else
            x = y->right;

    if (y->color == BLACK)
    {

        if (x->color == RED)
        {
            x->color = BLACK; // problem solved
            return;
        }

        int count = 0;
        if ((*tree)->size > 2)
        RB_Delete_Fixup(tree, x, rememberParent, &count);
    }
}




/*
PERFORMANCE + DEMO
*/

void doOps(int n)
{
    Operation opsDel = prof.createOperation("deleteDS", n);
    Operation opsSel = prof.createOperation("selectDS", n);
    Operation opsBuild = prof.createOperation("buildDS", n);
    RBType* tree = NULL;
    int countOps = 0;


    buildTree(&tree, 1, n, &countOps);
    opsBuild.count(countOps);


    for (int i = 1; i <= n; i++)
    {
        RBType* result = NULL;
        int index;
        do
        {
            countOps = 0;
            index = rand() % (n - 1) + 1;
            result = OS_Select(tree, index, &countOps);
            opsSel.count(countOps);
        } while (result == NULL);

        opsSel.count(countOps);

        countOps = 0;
        deleteRBT(&tree, result, &countOps);
        opsDel.count(countOps);

    }
}

void performance()
{
    for (int nbTest = 0; nbTest < NBS_TESTS; nbTest++)
    {

        for (int n = STEP_SIZE; n <= MAX_DIM; n++)
        {
            doOps(n);
        }
    }
    prof.divideValues("deleteDS", NBS_TESTS);
    prof.divideValues("selectDS", NBS_TESTS);
    prof.divideValues("buildDS", NBS_TESTS);
    prof.addSeries("semiTotal", "deleteDS", "selectDS");
    prof.addSeries("Total", "semiTotal", "buildDS");
    prof.showReport();
}

void prettyPrint(RBType* tree)
{
    if (tree != dummyNode)
    {
        for(int i = 0 ; i < getDepth(tree);i++)
            printf("\t");
        printf("K:%d", tree->key);
        printf(" D:%d C:", tree->size);
        (tree->color == RED) ? printf("R\n") : printf("B\n");
        prettyPrint(tree->left);
        prettyPrint(tree->right);
    }
}

void demo()
{

    RBType* tree = NULL;
    int n = 11;
    int aux = 0;
    buildTree(&tree, 1, n, &aux);
    preorder(tree);
    printf("\n________________\n");
    printf("\n________________\n");

    RBType* result;

    int index;
    for (int i = n; i >= 1; i--)
    {

        result = OS_Select(tree, i, &aux);
        printf("\n________________\n");
        printf("Se va sterge elemntul %d din RBT\n\n", result->key);
       
        deleteRBT(&tree, result,&aux);
        preorder(tree);

        printf("\n");
        prettyPrint(tree);
        printf("\n\n");
      
    }

    
    printf("done...\n");

  
}



int main()
{

    dummyNode = (RBType*)malloc(sizeof(RBType));
    dummyNode->color = BLACK; 
    dummyNode->size = 0;
    dummyNode->key = -1; 
    dummyNode->left = dummyNode->right = dummyNode->parent = NULL;
    
   
    srand(time(NULL));

    demo();
    //performance();
    
}