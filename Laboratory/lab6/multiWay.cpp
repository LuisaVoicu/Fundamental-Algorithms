/*
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
*
* ***** MULTI-WAY TREES *****
** Parent Representation . Multi-Way Representation. Binary Representation **
*
*
*/




/*
* 
* *** COMPLEXITATI
*                                       Cum stochez datele?                        Running Time?                                                                         Justificatie timp liniar                                                Additional Space
* _______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
*
* 1.Parent Representation (R1)          Array : key + level ( pentru PP )          setChildrensList O(n)     |                                                                                                                                     level -> O(n)
*                                                                                  initializare Array O(n)   | => overall O(n);
*                                                                                  setParent O(1) * n        |
*                                                                                  setLevel O(n^2) - doar pt PP -> nu decide complexitatea algoritmului!!!
* ________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
* 
* 2.MuliWay Representation (R1toR2)     Array de structuri ce contin               createMultiWay - O(1) * n => O(n)|=> O(n)                                             Stochez copii corespunzatori fiecarui                                      nbChild - > n 
*                                       -> key , nbChildrens , tabloul efectiv     R1toR2 - O(n)                    |                                                    nod intr-un array -> astfel nu mai e                                       childrens -> n*n ( as putea sa o aloc dinamic
*                                       -> + level (for PP)                                                                                                              nevoie sa parcurg intreg array-ul cu ParentRep                              a.i. sa reduc memoria folosita, dar ar insemna
*                                                                                                                                                                        (sunt obligata sa il parcurg full -- n*n->O(n^2)                            sa numar cati copii are fiecare nod... O(n^2) 
*                                                                                                                                                                        in total voi itera de n ori prin array-urile nodurilor                      deci e useless
*                                                                                                                                                                        n apeluri recursive si numar variabil de ieratii (dar total = n)            ==> overall O(n^2)
*                                                                                                                                                                           ===> O(n) 
*                                                                                                                                                                        OBS: am facut si un R1toR2 quadratic pentru verificare :) dar am si pe cel liniar                      
* ________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
*  
*3.Binaryrepresentation (R2toR1)       structura ce contine key, level(pt PP)      createBinRep --> O(1)*n -> O(n)                                                       Sa presupunem ca pentru un apel recursiv T(k)                                    key , level -> n 
*                                      + referinta la primul frate                 R3toR3 --> voi face exact n iteratii in total                                         o sa am un efort in afara apelului O(Xk)                                         leftChild, next/first/lastBrother -> n
*                                      ++ referinta la lista de frati              pentru n apeluri recursive -- demonstratie dreapta:                                   stiu ca X1+X2+...+Xn = n => O(n) efort total in afara apelurilor recursive       ...  ~ O(n)   (imbunatatit fara de R2 
*                                      (primul,ultimul si urmatorul)                ==> O(n)                                                                               inserare in lista cu referinta la ultimul si primul element : O(1)                      datorita implementarii cu liste)
*                                       a.i. sa inserez eficient in lista                                                                                                  T(n) = T(n) + T(n-1) + ... + T(1) + O(n)                  
*                                                                                                                                                                          ~ O(n) + O(n) ~ O(n)            
*
*/                                                                                                                                                                       


#include <stdio.h>
#include <stdlib.h>



// FUNCTII AUXILIARE //
//      LISTE       //


typedef struct NodeType {
    int key;
    struct NodeType* next;
}NodeType;

typedef struct {
    NodeType* first;
    NodeType* last;
}List;

NodeType* createNode(int key) {
    NodeType* node = (NodeType*)malloc(sizeof(NodeType));
    if (node == NULL) {
        printf("Nu poate fi alocata memorie!");
        exit(1);
    }
    node->key = key;
    node->next = NULL;
    return node;
}

void insertLast(List* list, int key) {
    NodeType* node = createNode(key);
    if (list->first == NULL) {
        list->first = list->last = node;
        return;
    }
    list->last->next = node;
    list->last = node;
}

void insertFirst(NodeType** first, int key)
{
    NodeType* node = createNode(key);
    node->next = *first;
    *first = node;
}

void showList(NodeType* first) {
    while (first != NULL) {
        printf("%d ", first->key);
        first = first->next;
    }
    printf("\n");
}



  //////////////////////////////////////
 //      PARENT REPRESENTATION       //
//////////////////////////////////////


typedef struct ParentRepresentation {
    int key;
    int level;
    NodeType* childrens;
}ParentRepresentation;

void prettyPrintR1(ParentRepresentation* parentArray, int n, int root) {

    for (int i = 0; i < parentArray[root].level; i++)
        printf("   ");
    printf("%d\n", root);

    for (int i = 0; i < n; i++)
    {
        if (parentArray[i].key == root)
        {
            prettyPrintR1(parentArray, n, i);
        }
    }

}

void showArray(ParentRepresentation* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" %d ", i);
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf(" %d ", arr[i].key);
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf(" %d ", arr[i].level);
    }
    printf("\n");
}


//O(n) =>  O(1) (insertFirst) * n pasi
void setChildrensList(ParentRepresentation* parentArray, int n) {
    for (int i = n - 1; i >= 0; i--)
        insertFirst(&parentArray[parentArray[i].key].childrens, i);
}

//O(1)
void setParentReprezentareParinte(ParentRepresentation* parentArray, int n, int parent, int child) 
{
    parentArray[child].key = parent;
}

// O(n^2) --> nu e tocmai relevanta pentru algoritm in sine, ci doar pentru pretty print
void getLevelReprezentareParinte(ParentRepresentation* parentArray, int n, int root)
{
    for (int i = 0; i < n; i++)
        if (parentArray[i].key == root)
        {
            parentArray[i].level = parentArray[root].level + 1;
            getLevelReprezentareParinte(parentArray, n, i);
        }
}

//O(n)
void initializeParentArray(ParentRepresentation* parentArray, int n) {
    for (int i = 0; i < n; i++) {
        parentArray[i].key = -1;
        parentArray[i].level = 0;
        parentArray[i].childrens = NULL;
    }
}


/*
*
* running time?
*       exceptant setLevel ( ce e relevant doar pentru pretty print ) avem : O(n) (initializare) + n * O(1) (setParent) + O(n) (setChildrensList) -> O(n)
* space  complexity ? ->
*       folosesc o lista pentru fiecare nod -> suma dimensiunilor tuturor listelor pentru fiecare nod din arbore = n => additional space = n => space complexity = O(n)
*       obs: stochez si informatie despre nivel, dar nu o folosesc in alg --> O(n) si tot ramane O(n) overall
*/

void demoR1() {
    // demo parent representation
    int parent_child[][2] = { {5,0}, {7,2} , {7,5}, {7,6} , {2,1} , {2,4}, {2,9}, {5,3}, {5,8}, {7,6} };
    int n = 10;
    int root = 7;
    /*int parent_child[][2] = { {7,0}, {3,2},{3,7},{3,5},{3,1},{2,4},{2,6},{5,8},{5,12},{4,9},{6,11},{6,13},{6,14},{6,15},{8,10} };
    int n = 16;
    int root = 3;*/

    ParentRepresentation* parentArray = (ParentRepresentation*)malloc(n * sizeof(ParentRepresentation));

    //O(n)
    initializeParentArray(parentArray, n);

    //n * O(1) 
    for (int i = 0; i < n-1; i++)
    {
        setParentReprezentareParinte(parentArray, n, parent_child[i][0], parent_child[i][1]);
    }

    //O(n)
    setChildrensList(parentArray, n);


    // operatiile de mai jos nu tin de algoritmul in sine
   /* for (int i = 0; i < n; i++)
    {
        printf("%d: ", i);
        showList(parentArray[i].childrens);
    }

    showArray(parentArray, n);*/


    getLevelReprezentareParinte(parentArray, n, root);

    // pretty print for Parent Representation
    printf("Parent Representation pretty print:\n");
    prettyPrintR1(parentArray, n, root);

}


  ///////////////////////////////////////
 //         MULTI WAY TREEES          //
///////////////////////////////////////

    /*
    *   Abordare a doua metode ... cu si fara stocarea copiilor 
    *   
    *   case 1. caut de fiecare data in array-ul de parinti copiii fiecarui nod ... repet pasul pentru fiecare nod => n pasi => efort in afara apelului recursiv = O(n) |  ==> n * O(n) = O(n^2)
    * 
    *                                                                                                                       complexitate pentru functia recursiva: O(n) |
    *   case 2. pentru ParentRepresentation voi memora si o lista de copii ... in total voi stoca dinamic n elemente 
    *           acum copii sunt stocati si e suficient sa parcurgem fiecare lista a fiecarui nod ( daca are o lista de copii )
    *           cum in toate nodurile sunt n elemente (total) se vor face n apeluri recursive
    *
    *       Complexity
    *       running time?  ( O(1) (create MultiWay node )  + O(1) (setLevel)) * n ( apel recursiv pentru fiecare dintre cele n noduri )  ~ O(n)   ==> running time
    * 
    *   Space Complexity? informatie in plus stocata?  -- nbChild (n), -- childrens -> in total se stocheaza dinamic n elemente in diferite liste ==>  ~O(n) space complexity
    *
    */


typedef struct MultiWay {
    int key;
    int level;
    int nbChild;
    MultiWay** childrens;
}MultiWay;

MultiWay* createMultiWay(int key, int n)
{

    MultiWay* node = (MultiWay*)malloc(sizeof(MultiWay));
    node->childrens = (MultiWay**)calloc(n , sizeof(MultiWay*));

    /*for (int i = 0; i < n; i++)
        node->childrens[i] = NULL;*/

    node->level = 0;
    node->key = key;
    node->nbChild = 0;
    return node;
}

void R1toR2Quadratic(MultiWay* tree, ParentRepresentation* parentArray, int n)
{
    // am pus deja radacina in demo
    for (int i = 0; i < n; i++)   // duce catre O(n^2) .... trebuie sa caut mereu copii in tot vectorul -> optimizare? pot pune pentru fiecare cheie copii intr-o lista in momentul in care construiect parent representation
    {
        if (parentArray[i].key == tree->key)
        {
            tree->childrens[tree->nbChild++] = createMultiWay(i, n); // O(1) 
            tree->childrens[tree->nbChild - 1]->level = tree->level + 1; // O(1)
            R1toR2Quadratic(tree->childrens[tree->nbChild - 1], parentArray, n);  //
        }
    }
}

void R1toR2Linear(MultiWay* tree, ParentRepresentation* parentArray, int n, int root)
{
    /*    
    *       Complexity
    *       running time?  ( O(1) (create MultiWay node ) + O(1) ( set Level) ) * n ~ O(n)
    *
    */

    if (parentArray[root].childrens != NULL)
    {
        NodeType* aux = parentArray[root].childrens;
        while (aux != NULL)
        {
            tree->childrens[tree->nbChild++] = createMultiWay(aux->key, n); // O(1) 
            tree->childrens[tree->nbChild - 1]->level = tree->level + 1; // O(1)
            R1toR2Linear(tree->childrens[tree->nbChild - 1], parentArray, n, aux->key);
            aux = aux->next;
        }
    }

}

void prettyPrintMW(MultiWay* tree)
{

    for (int i = 0; i < tree->level; i++)
    {
        printf("   ");
    }

    printf("%d\n", tree->key);

    for (int i = 0; i < tree->nbChild; i++)
    {
        prettyPrintMW(tree->childrens[i]);
    }


}


/*
*
*   running time? O(n) ( pentru R1toR2Linear )
*   space complexity?
*       external memory? nbChild , childrens[] =>  n + n * n => O(n^2)
*                        + level dar e folosit doar la afisare ( = O(n) deci nu influenteaza prea tare )
*
*/

void demoR2()
{
    // demo parent representation
    int parent_child[][2] = { {5,0}, {7,2} , {7,5}, {7,6} , {2,1} , {2,4}, {2,9}, {5,3}, {5,8}, {7,6} };
    int n = 10;
    int root = 7;
    /*int parent_child[][2] = { {7,0}, {3,2},{3,7},{3,5},{3,1},{2,4},{2,6},{5,8},{5,12},{4,9},{6,11},{6,13},{6,14},{6,15},{8,10} };
    int n = 16;
    int root = 3;*/

    ParentRepresentation* parentArray = (ParentRepresentation*)malloc(n * sizeof(ParentRepresentation));
    initializeParentArray(parentArray, n);
    for (int i = 0; i < n-1; i++)
    {
        setParentReprezentareParinte(parentArray, n, parent_child[i][0], parent_child[i][1]);
    }
    setChildrensList(parentArray, n);



    MultiWay* tree = createMultiWay(root, n);
    printf("MultiWay pretty Print quadratic algorithm:\n");
    R1toR2Quadratic(tree, parentArray, n);

    prettyPrintMW(tree);

    MultiWay* treeLinear = createMultiWay(root, n);

    R1toR2Linear(treeLinear, parentArray, n, root);
    printf("\nMultiWay pretty Print linear algorithm: ");
    printf("\n");
    prettyPrintMW(tree);

}


typedef struct BinaryRepresentation
{
    int key;
    int level;
    BinaryRepresentation* leftChild;
    BinaryRepresentation* nextBrother;
    BinaryRepresentation* firstBrother;
    BinaryRepresentation* lastBrother;

}BinaryRepresentation;






BinaryRepresentation* createBinRepNode(int key, int n)
{
    BinaryRepresentation* node = (BinaryRepresentation*)malloc(sizeof(BinaryRepresentation));
    node->key = key;
    node->level = 0;
    node->leftChild = node->firstBrother = node->lastBrother = node->nextBrother = NULL;
    return node;
}


void insertLastBrother(BinaryRepresentation** binTree, BinaryRepresentation* brother)
{
    if ((*binTree)->firstBrother == NULL)
    {
        (*binTree)->firstBrother = (*binTree)->lastBrother = brother;
        return;
    }
    (*binTree)->lastBrother->nextBrother = brother ;
    (*binTree)->lastBrother = brother;
}



/*
*       Complexity?
*       Running time
*        -->  numarul total de iteratii cu for  = x <  n  => x apeluri recursive pentru setarea array-ului de frati |
*                                                      y - nr apeluri recursive pentru searea pointerului leftChild | ==> x + y = n apeluri recursive                  |
*                                                                                                                      Efort in afara apelurilor recursive ... O(1)    | => O(n) space complexity
*                                                        pentru ca inserare la sfarsit  intr-o lista unde se cunoaste adresa primului si ultimului element ... O(1)
*       
*       Space Complexity for R2 to R3 transformation?  ( nu tinem cont de nivel ce e folosit doar la pretty print)
*           -> nextBrother , firstBrother, lastBrother
*  
*/

BinaryRepresentation*  R2toR3(MultiWay* tree,int n, char l)
{
   
    if (tree != NULL)
    {
           BinaryRepresentation* binTree = createBinRepNode(tree->key,n);        
           binTree->leftChild = R2toR3(tree->childrens[0], n, 'l');
          
            for (int i = 1; i < tree->nbChild; i++)  
            {
                BinaryRepresentation* aux = R2toR3(tree->childrens[i], n, 'r');
                insertLastBrother(&(binTree->leftChild), aux);
                
            }

        return binTree;
    }
    else
    {   
        return NULL;
    }
    
}

void getLevelForR3Rep(BinaryRepresentation* binTree)
{
    if (binTree != NULL )
    {
        if (binTree->leftChild != NULL) 
        {
            binTree->leftChild->level = binTree->level + 1;
            getLevelForR3Rep(binTree->leftChild);
        }

        BinaryRepresentation* aux = binTree->firstBrother;

        while (aux != NULL)
        {
            aux->level = binTree->level;
            getLevelForR3Rep(aux);
            aux = aux->nextBrother;
        }
    }
}

void prettyPrintBR(BinaryRepresentation* binTree)
{
    if (binTree != NULL)
    {
        for (int i = 0; i < binTree->level; i++)
        printf("   ");
        printf("%d\n", binTree->key);

        prettyPrintBR(binTree->leftChild);
        BinaryRepresentation* aux = binTree->firstBrother;
        while (aux != NULL)
        {
            prettyPrintBR(aux);
            aux = aux->nextBrother;
        }
    }
}

void demoR3()
{
    /*int parent_child[][2] = { {5,0}, {7,2} , {7,5}, {7,6} , {2,1} , {2,4}, {2,9}, {5,3}, {5,8}, {7,6} };
    int n = 10;
    int root = 7;*/

    int parent_child[][2] = { {7,0}, {3,2},{3,7},{3,5},{3,1},{2,4},{2,6},{5,8},{5,12},{4,9},{6,11},{6,13},{6,14},{6,15},{8,10} };
    int n = 16;
    int root = 3;
    ParentRepresentation* parentArray = (ParentRepresentation*)malloc((n + 1) * sizeof(ParentRepresentation));

    //O(n)
    initializeParentArray(parentArray, n);

    //n * O(1) 
    for (int i = 0; i < n-1; i++)
    {
        setParentReprezentareParinte(parentArray, n, parent_child[i][0], parent_child[i][1]);
    }

    //O(n)
    setChildrensList(parentArray, n);

    MultiWay* tree = createMultiWay(root, n);
    R1toR2Linear(tree, parentArray, n, root);

    BinaryRepresentation* binTree = R2toR3(tree, n,'a');    
    printf("\Binary representation from MultiWay representation pretty Print:\n");

    getLevelForR3Rep(binTree);
    prettyPrintBR(binTree);

}

int main()
{

    demoR1();
    printf("\n____________________________________________________\n");
    demoR2();
    printf("\n____________________________________________________\n");
    demoR3();


}