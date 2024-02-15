/*
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
* 
* ***** HASH TABLES. *****
* * Implementare operatii INSERT, SEARCH , DELETE . Analiza in functie de factorul de umplere**
*
* 
*/

/*
                                            COMPLEXITATI 
                        RUNNING TIME                            SPACE COMPLEXITY
                WORST       BEST        AVERAGE                    ALL: O(n)
INSERT:         O(n)        O(1)        O(1)

SEARCH:         O(n)        O(1)        O(1)

DELETE:         O(n)        O(1)        O(1)

                        


*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define NB_TESTS 5
#define MAX_SIZE_KEY 40

Profiler prof("HashTable");

enum Status {FREE, OCCUPIED};

const int c1 = 1;
const int c2 = 1;

typedef struct KeyFields
{
    int id;
    char* name;  

}KeyFields;

typedef struct Cells
{
    KeyFields key;
    int size;
    Status status;
}Cells;

typedef struct
{
    int effort;
    int slot;
}EffortSlot;




int minimOfThree(int a, int b, int c)
{
    int minim = a;
    if (minim > b)
    {
        minim = b;
    }
    if (minim > c)
    {
        minim = c;
    }
    return minim;
}

char* convertIntegerToString(int n)
{
    char* s = (char*)calloc(100, sizeof(char));
    int sizeS = 0;
    while (n)
    {
        s[sizeS++] = n % 10 + '0';
        n /= 10;
    }
    for (int i = 0; i < sizeS / 2; i++)
    {
        char aux = s[i];
        s[i] = s[sizeS - i - 1];
        s[sizeS - i - 1] = aux;
    }
    return s;
}



void initializeHashTable(Cells* table, int N)
{
    for (int i = 0; i < N; i++)
    {
        table[i].status = FREE;
    }
}

void showTable(Cells* table, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i].status == FREE)
        {
            printf("%d: _FREE_ /  ", i);
        }
        else
        {
            printf("%d:%d / ", i, table[i].key.id);
        }
    }
    printf("\n");
}

KeyFields makeKey(int id)
{
    KeyFields element;
    element.id = id;
    element.name = convertIntegerToString(id);
    return element;
}

bool equalKeys(KeyFields key1, KeyFields key2)
{
    // aici poate fi generalizat in functie de datele de intrare

    if (key1.id != key2.id)
    {
        return false;
    }

    return true;
}

void insertKeyInTable(Cells* table, int slot, KeyFields newKey) // poate nu modifica vectorul??
{
    table[slot].key.name = newKey.name;
    table[slot].status = OCCUPIED;
    table[slot].key.id = newKey.id;
 
}



int getHashCode(int id)
{
    return id;
}

int getQuadraticProbing(KeyFields newKey, int i, int N)
{
    int h = getHashCode(newKey.id);

    return (h + c1 * i + c2 * i * i) % N;
    
}





                                  /*************************
                                 *  OPERATII HASH-TABLE   *
                                *************************/

int insertHashTable(Cells* table, int N,  KeyFields newKey) // bonus:  returneaza efortul ( i )  de inserare :D
{
    int i = 0;
    int slot = getQuadraticProbing(newKey, i, N);
    int collision = slot;
    if (table[slot].status == FREE)
    {
        insertKeyInTable(table, slot, newKey);
        return i;
    }

    do{

        i++;
        slot = getQuadraticProbing(newKey, i, N);

    } while (slot != collision && table[slot].status == OCCUPIED);

    if (table[slot].status == FREE)
    {
        insertKeyInTable(table, slot, newKey);
        return i;
    }

    return -1; // nu a putut fi inserat!

}

EffortSlot searchHashTable(Cells* table, int N, KeyFields searchedKey) // returneaza efortul +  slotul in care e gasit elementul
{
    EffortSlot result;
    result.effort = 0;
    result.slot = getQuadraticProbing(searchedKey, result.effort, N);;
    int collision = result.slot;

    do
    {

        if (table[result.slot].status == OCCUPIED && equalKeys(searchedKey, table[result.slot].key) == true)
        {

            return result;
        }

        result.effort++;
        result.slot = getQuadraticProbing(searchedKey, result.effort, N);
    } while (collision != result.slot && table[result.slot].status != FREE);

    if (collision == result.slot || table[result.slot].status == FREE)
    {
        result.slot = -1;
        return result; // result = (-1,-1) nu am gasit cheia cautata -> fie m-am intors de unde am plecat, fie am dat de un slot liber... la urmatoarele cautari clar vor fi libere si celalalte deci e useless sa continuam cautarea;
    }
}

void deleteHashTable(Cells* table, int N, KeyFields deleteKey)
{
    // nu implementam efectiv stergerea unui element din tabela ( ar fi prea costisitor si se urmareste implementarea operatiei in O(1) ) , ci doar marcam slotul ocupat ca fiind neocupat :)
    // stergerea unui element presupune CAUTAREA lui si marcarea statusului ca fiind OCCUPIED;
  
    EffortSlot resultDelete = searchHashTable(table, N, deleteKey);

    if (resultDelete.slot == -1)
    {
        printf("\nCheia %d nu exista in tabela de dispersie deci nu poate fi stearsa! \n", deleteKey.id);
        return;
    }

    table[resultDelete.slot].status = FREE; // am eliberat slotul ; 


}


KeyFields* generateExistingElementsInHashTable(int N, int m, int countEfforts, int* frequencyEfforts, Cells* table)
{

    KeyFields* existInTable = (KeyFields*)malloc(m / 2 * sizeof(KeyFields));
    int sizeExist = 0;
    int rest = m / 2;
    /*
    din hashTable voi lua maxim 'stop' (in principiu este egal cu x = m/(2*countEfforts) DAR se poate sa am mai putine eforturi de aceeasi valoare decat ce am ales pentru x ) elemente ce au frecventa indexFrequency 
            -> astfel voi lua pe rand fiecare frecventa in pare de un numar de ori ( x daca frecventa e mai mare decat el, altfel frecventa in sine)-> asigur astfel uniformitate
                    -> ce fac totusi daca ajung la finalul tabelei si eu inca nu am selectat m/2 elemente? R: iau lista de la capat si procedez la fel pana cand am gasit m/2 elemente in total in tabela de hash
    */
    do
    {
        for (int indexFrequency = 0; indexFrequency < N && rest > 0; indexFrequency++)
            if (frequencyEfforts[indexFrequency])
            {

                int count = 0;
                int stop = minimOfThree(m / (2 * countEfforts), frequencyEfforts[indexFrequency], rest);

       
                frequencyEfforts[indexFrequency] -= stop;
                rest -= stop;
                for (int i = 0; i < N && count < stop; i++) 
                {
                    if (table[i].status == OCCUPIED && searchHashTable(table, N, table[i].key).effort == indexFrequency)
                    {
                            //printf("intri aici? ");
                            count++;
                            existInTable[sizeExist++] = table[i].key;

                    }
                }

            }
    } while (rest > 0);

    return existInTable;
}

void demoInserare() 
{
  // Implementarea operației de inserare într-o tabela de dispersie; demo pe factor de umplere 95 %

    /*
    aleg  N = 19 => n/N = 95 / 100 => n = 19 * 95 / 100 ~ 18
    */

     
        printf("\n____ DEMO INSERARE IN HASHTABLE ____\n\n");

    int N; // nr de sloturi disponibile in table
    int n; // nr de sloturi ocupate
    int nbOfInsertedValues = 0;

    printf("Introduceti numarul total de sloturi ale tabelei. N = ");
    scanf("%d", &N);

    n =( 95 * N )/ 100 ;
    printf("Factor de umplere = 95 \% => n = %d .\n", n);
    Cells* table = (Cells*)malloc(sizeof(Cells) * N);
    initializeHashTable(table, N);

    int* collisionElements = (int*)calloc(N , sizeof(int)); // stochez elementele ce nu au putut fi introduse in tabela
    int sizeCollision = 0;


    printf("\nNu pot fi introduse urmatoarele elemente in tabela: \n");
    do
    {
        int idValue = rand() % (N * n - 1);  // valoare random din intervalul (0,N*n)
      //  printf("%d ", idValue);
        int inserted = insertHashTable(table, N, makeKey(idValue));
        if (inserted != -1)
        {
            nbOfInsertedValues++;
        }
        else
        {
            printf("%d ( first slot :  %d ), ", idValue, getQuadraticProbing(makeKey(idValue),0, N));
        }

    } while (nbOfInsertedValues < n);
    printf("\n\n");
    printf("Tabela are urmatoarea forma: \n");
    showTable(table, N);

}

void demoCautare()
{
    printf("\n\n____ DEMO CAUTARE IN HASHTABLE ____\n\n");

    // inseram rapid
    int N;
    float alpha;

    printf("Introduceti numarul total de sloturi ale tabelei. N = ");
    scanf("%d", &N);

    printf("Introduceti factorul de umplere (eg. pentru 20 la suta veti introduce 0.2). Alpha = ");
    scanf("%f", &alpha);


    int n = alpha * N;

    printf("Vor fi introduse %d numere in tabela. \n ", n);
    int nbOfInsertedValues = 0;
    Cells* table = (Cells*)malloc(sizeof(Cells) * N);
    int* rememberToSearch = (int*)(N * sizeof(int));
    initializeHashTable(table, N);

    do
    {
        int idValue = rand() % (N * n - 1);  
        int inserted = insertHashTable(table, N, makeKey(idValue));
        if (inserted != -1)
        {
            nbOfInsertedValues++;
            
        }
    } while (nbOfInsertedValues < n);

    printf("Elementele din HashTable sunt: \n");
    showTable(table, N);
    printf("\n____________________________________________________________________________\n");
    int stillSearching = 0 ;
    do
    {
        int search = 0;
        printf("Introduceti elementul pe care il cautati: ");
        scanf("%d", &search);
        EffortSlot result = searchHashTable(table, N, makeKey(search));

        (result.slot == -1) ? printf("Elementul %d nu a fost gasit !\n", search) : printf("Elementul %d a fost gasit pe slotul %d cu efortul %d. \n", search, result.slot, result.effort);

        printf("Continuati testarea? 0/1 -> ");
        scanf("%d", &stillSearching);

    } while (stillSearching == 1);


}


int* fillHashTable(Cells* table, int n, int N, int* countEfforts)
{
    int* frequencyEfforts = (int*)calloc(N, sizeof(int)); // efortul nu poate depasi numarul de elemente inserate 
    int nbIns = 0;

    do
    {
        int idValue;
        EffortSlot result;
        do
        {   // caut element ce nu se afla deja in lista -> in lista voi avea doar elemente distrincte
            idValue = rand() % (n * N * N - 1);
            result = searchHashTable(table, N, makeKey(idValue));

        } while (result.slot != -1);

        int inserted = insertHashTable(table, N, makeKey(idValue));



        if (inserted != -1)
        {

            if (frequencyEfforts[inserted] == 0)
            {
                (*countEfforts)++;    //return;

            }
            frequencyEfforts[inserted]++;
            nbIns++;
        }
    } while (nbIns < n);
    return frequencyEfforts;
}

void doMSearch(Cells* table, int N, int n, int countEfforts, int* frequencyEfforts)
{


    int m = 3000;
    float avgNFTests = 0;
    float avgFTests = 0;
    float maxNFTests = 0;
    float maxFTests = 0;
   
    for (int tests = 0; tests < NB_TESTS; tests++)
    {
        int countNotFoundElements = 0;
        float averageEffortNotFound = 0;
        float maxEffortNotFound = 0;
        float averageEffortFound = 0;
        float maxEffortFound = 0;

        KeyFields* existInTable = generateExistingElementsInHashTable(N, m, countEfforts, frequencyEfforts, table);
        for (int i = 0; i < m / 2; i++)
        {
            int effort = searchHashTable(table, N, existInTable[i]).effort;
            averageEffortFound += effort;
            if (effort > maxEffortFound)
            {
                maxEffortFound = effort;
            }

            //printf("%d %d\n", existInTable[i].id, effort);
        }
        averageEffortFound /= (m / 2);

        /*
        * pentru a determina efortul maxim depus pentru a gasi un element ce nu se afla hashTable voi lua atat elemente din afara intervalului ( adica din [ N*n + 1 , N*n*n] de exemplu ) cat si din interior;
        */

        do
        {
            int idValue = rand() % (N * n * N - n * N - 1 - 1) + N * n + 1;
            //printf("~ %d ", idValue);
            EffortSlot resultSearch = searchHashTable(table, N, makeKey(idValue));
            //printf("%d \n", effort);

            averageEffortNotFound += resultSearch.effort;
            if (resultSearch.effort > maxEffortNotFound)
            {
                maxEffortNotFound = resultSearch.effort;
            }

            countNotFoundElements++;
        } while (countNotFoundElements < m / 4);

        do
        {
            EffortSlot resultSearch;
            int idValue;
            do
            {
                idValue = rand() % (N * n - 1);
                resultSearch = searchHashTable(table, N, makeKey(idValue));
            } while (resultSearch.slot != -1);

            //printf("%d %d\n", idValue, resultSearch.effort);
            averageEffortNotFound += resultSearch.effort;
            if (resultSearch.effort > maxEffortNotFound)
            {
                maxEffortNotFound = resultSearch.effort;
            }

            countNotFoundElements++;
        } while (countNotFoundElements < m / 4);

        averageEffortNotFound /= countNotFoundElements;

        avgNFTests += averageEffortNotFound;
        avgFTests += averageEffortFound;
        maxNFTests += maxEffortNotFound;
        maxFTests += maxEffortFound;
    }

    avgNFTests /= NB_TESTS;
    avgFTests /= NB_TESTS;
    maxFTests /= NB_TESTS;
    maxNFTests /= NB_TESTS;
    printf("        %5.2f        |        %5.2f        |        %5.2f        |        %5.2f        |\n", avgFTests, maxFTests, avgNFTests, maxNFTests);
    printf("|___________________|____________________|____________________|______________________|______________________|\n");

}

void analizaEfortCautare()
{
   /*
   cum gasim acele 1500 de elemente deja existente in tabela a.i. ele sa fie distribuite uniform?
   -> calculam frecventa fiecarui efort determinat la inserare
   -> initial ne asiguram ca printre elementele cautate se vor regasi cate un element corespunzator fiecarui efort determinat
   -> repetam de la 0 -> efortMaxim pana cand determinam 1500 de elemente

   -> deci... daca avem x eforturi , in sirul de elemente cautate se vor gasi 1500 / x valori corespunzatoare FIECARUI efort
                                         --> aici poate aparea problema ca frecventa efortului sa fie mai mica
                                               --> in acest caz, doar "pasez" restul la urmatorul
                                                     --> daca totusi ajung la finalul sirului de frecventa si inca mai am elemente o iau de la 0
   
   */

    srand(time(NULL));

    int N = 10007; // aici pune 10007
    float alpha[6] = {0.8, 0.85, 0.9, 0.95, 0.99};
    int m = 3000; // elemente de cautat -> 15000 gasite , 15000 negasite 


    printf("\n___________________________________________  ANALIZA - CAUTARE  _____________________________________________");

    printf("\n_____________________________________________________________________________________________________________\n");
    printf("|   FACTOR DE       |     EFORT MEDIU    |      EFORT MAXIM   |      EFORT MEDIU     |      EFORT MAXIM     |\n");
    printf("|    UMPLERE        |       GASITE       |        GASITE      |        NEGASITE      |        NEGASITE      |\n");
    printf("|___________________|____________________|____________________|______________________|______________________|\n");

    for (int alphaIndex = 0; alphaIndex < 5; alphaIndex++)
    {
        Cells* table = (Cells*)malloc(sizeof(Cells) * N);
        initializeHashTable(table, N);
        int countEfforts = 0;
        int n = N * alpha[alphaIndex];
        int* frequencyEfforts = fillHashTable(table, n, N, &countEfforts);

        //showTable(table, N);
        printf("|       %.2f        |", alpha[alphaIndex]);
        
        doMSearch(table, N, n, countEfforts, frequencyEfforts);
      
    }
}

enum TypeOfCall { TESTING, EVALUATE };

void demoStergere(TypeOfCall typeCall)
{
    srand(time(NULL));

    printf("\n\n____ DEMO STERGERE IN HASHTABLE ____\n\n");

    int N;
    int n;
    float alpha = 0.99;

    if (typeCall == TESTING)
    {
        printf("Introduceti dimensiunea maxima a tabelei de dispersie. N = ");
        scanf("%d", &N);
    }
    else
    {
        N = 10007;
    }

    printf("\nN = %d\n", N);
    n = N * alpha;
    printf("\nFactor de divizare = %g\n", alpha);
    printf("Numarul de elemente inserate in tabela. n = %d\n", n);

    int nbOfInsertedValues = 0;
    Cells* table = (Cells*)malloc(sizeof(Cells) * N);
    initializeHashTable(table, N);

    int countEfforts = 0;
    int* frequencyEfforts = fillHashTable(table, n, N, &countEfforts);

    if (typeCall == TESTING)
    {
       showTable(table, N);
    }
    
    alpha = 0.8;
    int nbOfDeletedKeys = n - N * alpha;


    
    printf("Vor fi sterge %d elemente din tabela.\n", nbOfDeletedKeys);
    
    // stergerea efectiva a elementelor din tabel // ne asiguram sa scadem frecventa unde e cazul
    while (nbOfDeletedKeys > 0)
    {
        EffortSlot toBeDeleted;
        int idValue;
        do
        {
            idValue = rand() % (N * N * n - 1);
            toBeDeleted = searchHashTable(table, N, makeKey(idValue));
        } while (toBeDeleted.slot == -1);
        
        if (typeCall == TESTING)
        {
            printf("Va fi stearsa cheia cu ID-ul %d de pe slotul %d.\n", idValue, toBeDeleted.slot);
        }
        deleteHashTable(table, N, makeKey(idValue));

        frequencyEfforts[toBeDeleted.effort]--;
        if (frequencyEfforts[toBeDeleted.effort] == 0)
        {
            countEfforts--;
        }
        
       // printf("--> %d %d \n", toBeDeleted.effort, frequencyEfforts[toBeDeleted.effort]);
        nbOfDeletedKeys--;
    }

    if (typeCall == TESTING)
    {
        showTable(table, N);
    }



    // EVALUATE -> cautare elemente dupa stergere

    if (typeCall == EVALUATE)
    {

        printf("\n___________________________________________  ANALIZA - CAUTARE DUPA STERGERE ________________________________");

        printf("\n_____________________________________________________________________________________________________________\n");
        printf("|   FACTOR DE       |     EFORT MEDIU    |      EFORT MAXIM   |      EFORT MEDIU     |      EFORT MAXIM     |\n");
        printf("|    UMPLERE        |       GASITE       |        GASITE      |        NEGASITE      |        NEGASITE      |\n");
        printf("|___________________|____________________|____________________|______________________|______________________|\n");
        printf("|       %.2f        |", alpha);


        doMSearch(table, N, n, countEfforts, frequencyEfforts);
    }
}


int main()
{


    int notBoredYet = 0;
    do
    {
        printf("Operatii:\n1: Demo inserare in HashTable;\n2: Demo cautare in HashTable;\n3: Analiza Efort Cautare in HashTable;\n4: Demo Stergere in HashTable;\n5: Demo Analiza Cautari dupa stergere in...HashTable :D\n Alege una dintre operatii!\n");
        int op;
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            demoInserare();
            break;
        case 2:
            demoCautare();
            break;
        case 3:
            analizaEfortCautare();
            break;
        case 4:
            demoStergere(TESTING);
            break;
        case 5:
            demoStergere(EVALUATE);
            break;

        }
        printf("Doresti sa efectuezi alta operatie? 1/0: ");
        scanf("%d",&notBoredYet);
    } while (notBoredYet);

}