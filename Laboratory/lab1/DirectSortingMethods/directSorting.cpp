/**
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
* 
* METODE DIRECTE DE SORTARE : SELECTION SORT, INSERTION SORT, BUBBLE SORT
* -- Implementare. Analiza complexitate
* 
*/

/*
* 
Concluzii:
SPACE COMPLEXITY: O(1) -> pentru bubble, selection, classicInsertion
                  O(log n) -> pentru binarySearchInsertion --> (binary search e implementat recursiv deci ocupam memorie pe stiva)

TIME COMLEXITY: 
- AVERAGE
                       Atriburi:    Comparatii:       Total:
 Selection               O(n)       O(n²)             O(n²)
 Bubble                  O(n²)      O(n²)             O(n²)
 Insertion(classic)      O(n²)      O(n²)             O(n²)
 Insertion(binary)       O(n²)      O(nlogn)          O(n²) dar evitent se fac mai puține operații

- WORST
                       Atriburi:    Comparatii:       Total:
 Selection               O(n)       O(n²)             O(n²)
 Bubble                  O(n²)      O(n²)             O(n²)
 Insertion(classic)      O(n²)      O(n²)             O(n²)
 Insertion(binary)       O(n²)      O(nlogn)          O(n²)

- BEST
                       Atriburi:    Comparatii:       Total:
 Selection               O(1)       O(n²)             O(n²)
 Bubble                  O(1)       O(n)              O(n)
 Insertion(classic)      O(n)       O(n)              O(n)             // NOTA: la atribuiri am timp liniar pentru ca , chiar daca array-ul e sortat
 Insertion(binary)       O(n)       O(nlogn)          O(nlogn)        // tot trebuie sa am 'rememberValue = arr[i]' pentru a sti ce pozitie caut ( chiar daca gasesc aceeasi valoare) 
 *
*/


#include <stdio.h>
#include "Profiler.h"

Profiler p("directSorting");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NBS_TESTS 5

void swapping(int* a, int* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}


void selectionSort(int arr[], int n)
{

    /*
    selectionSort in a nutshell :D
    -> array-ul e impartit in 2 subsiruri: cel mai din stanga sortat (S), iar cel mai din dreapta nesortat (D)
    -> selectionSort cauta in D cel mai mic element si il interschimba cu primul element din D
    -> astfel length(S) creste pana cand ajunge la length(Array) 
    */

     /* stabilitate:    NU -> exemplu : 3(1), 3(2), 1 ==> 1, 3(2), 3(1)
     * 
     *  adaptavilitate: NU -> numarul de comparatii ramane O(N^2)
     */
    Operation opComp = p.createOperation("selection-comp",n);
    Operation opAttr = p.createOperation("selection-attr",n);

    for (int i = 0; i < n - 1; i++)
    {
        int minPosition = i; // pozitia i - (probabil) element nesortat din array

        for (int j = i + 1; j < n; j++)
        { 
            //iterez prin Array de n - i + 1 ori pana cand gasesc elementul cel mai mic
                                              // evident, mai mare sau egal decat arr[i]
            opComp.count();
            if (arr[j] < arr[minPosition]) 
            {
                minPosition = j;
            }
        }

        if (minPosition != i)
        {
            //am gasit pozitia elementului mai mic sau egal decat arr[i] -> swap
            // la swap se fac 3 atribuiri
            opAttr.count(3);
            swapping(&arr[i], &arr[minPosition]);
        }
    }
}


void bubbleSort(int arr[], int n)
{

    /*
     * stabilitate? DA    -> e asigurata de faptul ca la (1) avem strict '>'
     * adaptavilitate? DA -> daca sirul e sortat (~ best case) complexitatea algoritmului se reduce la O(n)
     * 
     */

    /*
    * bubbleSort - how it works?
    * -> aducem cel mai mare element pe pozitia n (care initial e egala cu length[Array]) din sir -> pozitia n e updatata la n-1 
    * -> pentru elementul i vom parcurge array-ul de la 0 la n - 2 ==> fac ( in cel mai rau caz , cand sirul e sortat descrescator ) (n-1)*n/2
    */
    Operation opComp = p.createOperation("bubble-comp", n);
    Operation opAttr = p.createOperation("bubble-attr", n);

    int stillUnsorted;
    int lengthArray = n;
   
    do
    {
        stillUnsorted = 0; // parcurgem sirul cat timp gasim macar o pereche elemente nesortate
        
        for (int i = 0; i < lengthArray - 1; i++)  // de fiecare data cand parcurgem sirul initial vom parcurge de fapt cu un pas mai putin 
                                                                // ( ordonarea se face de la dreapta la stanga deci nu are sens sa mai comparam elementele deja ordonate)
        {
            opComp.count();
            if (arr[i] > arr[i + 1]) //(1)
            {
                opAttr.count(3);
                swapping(&arr[i], &arr[i + 1]);
                stillUnsorted = 1; // am facut o schimbare deci poate inca nu e sortat
            }
        }

        lengthArray--; // eficientizare ( nu are sens sa mergem pana la finalul sirului pentru ca deja am pus acolo cea mai mare  valoare la fiecare pas )

    } while (stillUnsorted);

}


void classicInsertionSort(int arr[], int n)
{
    /*
     * stabilitate?    -> DA -> asigurata de conditia (1) strict mai mare! '>'
     * adaptavilitate? -> DA -> daca sirul e sortat (~ best case) complexitatea algoritmului se reduce la O(n)
     */

    /*
    * 
    * sortarea prin inserare: subsirul din stanga sortat (S) si subsirul din dreapta nesortat(D)
    * la insertionSort face cautari in sirul deja sortat S ( spre deopsebire de selectionSort ce face cautari in sirul D)
    * se cauta pozitia pe care elementul curent ( din sirul D) o are in sirul S
    * 
    */
    Operation opComp = p.createOperation("classicInsertion-comp", n);
    Operation opAttr = p.createOperation("classicInsertion-attr", n);

    for (int i = 1; i < n; i++)
    {
        opAttr.count();
        int rememberValue = arr[i]; // elementul pentru care trebuie sa cautam pozitia in subsirul S
        int j;
        for (j = i - 1; j >= 0 && arr[j] > rememberValue; j--) // arr[j] > rememberValue -> eficientizare (1)
                                   //nu are sens sa continuam daca am gasit o valoare mai mica ( toate restul vor fi si mai mici pentru ca ne aflam in sirul sortat)(1)
        {
            opComp.count();
            opAttr.count();
            arr[j + 1] = arr[j];  // mutam fiecare element spre dreapta pentru a-i face loc elementului mai mic decat acestea
        }
        if (j >= 0)
        {
            // sigur am iesit din cauza comparatiei arr[j] > rememberValue ---> deci comparatia a fost facuta, chiar daca avem false ca rezultat
            opComp.count();
        }
        if (j + 1 != i)
        {
            opAttr.count();
            arr[j + 1] = rememberValue;
        }
    }
}


int binarySearch(int arr[], int left, int right, int toBeFound, Operation* opComp)
{
    if (left >= right)
    {
        return left;
    }

    else {
        int middle = (left + right) / 2;

        (*opComp).count();
        if (toBeFound < arr[middle])
        {
            return binarySearch(arr, left, middle, toBeFound, opComp);
        }
        else
        {
            return binarySearch(arr, middle + 1, right, toBeFound, opComp);
        }
    }
}


void binarySearchInsertionSort(int arr[], int n)
{
    /*
     * stabilitate? -> DA    --> asigurata din binary Search --> binary search : ~ daca nu gaseste numarul cautat, returneaza pozitia urmatorului numar mai mare 
     *                                                                           (de ex daca in sir cauta 3 si nu exista, dar il are 4 in sir, va returna pozitia lui) 
     *                                                                           ~ daca gaseste numarul cautat in sir , va returna pozitia urmatorului numar mai mare 
     *                                                                           (de ex 3,3,3,4 (cauta 3) => returneaza 3 ( pozitia lui 3)) deci pe pozitia 3 va inserat un NOU 3
     *                                                                                  (care evitent in sir se afla "mai la dreapta" fata de celilalti 3 din sir) , astfel e asigurata stabilitatea
     * adaptavilitate? -> DA --> pentru best case obtinem complexitate O(nlogn)
     * 
     */
    
    /*
    * ideea e aceeasi ca si la classicInsertionSort , doar ca partea de cautare nu va fi liniara, ci logaritmica datorita binarySearch-ului :)
    */

    Operation opComp = p.createOperation("binarySearchInsertion-comp", n);
    Operation opAttr = p.createOperation("binarySearchInsertion-attr", n);

    for (int i = 1; i < n; i++)
    {   
        int insertPosition = binarySearch(arr, 0, i, arr[i], &opComp);
        opAttr.count();
        int rememberValue = arr[i];

        for (int j = i; j > insertPosition; j--)
        {
            opAttr.count();
            arr[j] = arr[j - 1];
        }

        if (insertPosition != i)
        {
            opAttr.count();
            arr[insertPosition] = rememberValue;
        }
    }
}



void showArray(int v[], int n, int isSorted)
{
    (isSorted) ? printf("After sorting: ") : printf("Before sorting: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void copyArray(int v[], int vCopy[], int n)
{
    for (int i = 0; i < n; i++)
    {
        vCopy[i] = v[i];
    }
}


void demo()
{
    // selection sort
    printf("\n Selection Sort:\n");
    int testSelection[] = { 3,-12,45,3,90,14,26,0 };
    int nSel = sizeof(testSelection) / sizeof(int);

    showArray(testSelection, nSel, 0);
    bubbleSort(testSelection, nSel);
    showArray(testSelection, nSel, 1);

    // bubble sort
    printf("\n Bubble Sort:\n");
    int testBubble[] = { 3,-12,45,3,90,14,26,0 };
    int nBub= sizeof(testBubble) / sizeof(int);

    showArray(testBubble, nBub, 0);
    selectionSort(testBubble, nBub);
    showArray(testBubble, nBub, 1);    
    
    // classic insetion sort
    printf("\n Classic Insertion Sort:\n");
    int testClassicInsertion[] = { 3,-12,45,3,90,14,26,0 };
    int nCIns= sizeof(testClassicInsertion) / sizeof(int);

    showArray(testClassicInsertion, nCIns, 0);
    classicInsertionSort(testClassicInsertion, nCIns);
    showArray(testClassicInsertion, nCIns, 1);  

    // binary search insetion sort
    printf("\n Binary Search Insertion Sort:\n");
    int testBSInsertion[] = { 3,-12,45,3,90,14,26,0 };
    int nBSIns= sizeof(testBSInsertion) / sizeof(int);

    showArray(testBSInsertion, nBSIns, 0);
    binarySearchInsertionSort(testBSInsertion, nBSIns);
    showArray(testBSInsertion, nBSIns, 1);

}



void makeItWorstForSelection(int v[], int n)
{
    // tin cont ca v e descrescator
    int auxArray[MAX_SIZE];

    auxArray[0] = v[0];

    for (int i = 1; i < n; i++)
    {
        auxArray[i] = v[n - i];
    }
    copyArray(auxArray, v, n);
}



enum CASES {AVERAGE, WORST, BEST};


void performance(int order, CASES caseSelection)
{
    int arr[MAX_SIZE];
    int arrCopy[MAX_SIZE];

    int lengthArray;

    for (lengthArray = STEP_SIZE; lengthArray <= MAX_SIZE; lengthArray += STEP_SIZE)
    {
        for (int nbTests = 0; nbTests <= NBS_TESTS; nbTests++)
        {
                FillRandomArray(arr, lengthArray, 10, 50000, false, order);
            
           
                copyArray(arr, arrCopy, lengthArray);

                /*
                * Selection Sort
                * 
                   1 - average = > sir generat random     -->  order = UNSORTED deci nu fac modificari
                   2 - worst => sir rotit (eg. 5 1 2 3)   -->  deci order = DESCENDING (e mai convenabil pentru celalalte metode :) , astfel le pot pune intr-un singur grafic) si folosim o functie pentru a obtine forma "rotata"
                   3 - best => sir sortat crescator       -->  order = ASCENDING si nu mai modific nimic la array
                * 
                * 
                */

                if (caseSelection == WORST)
                {
                    makeItWorstForSelection(arr, lengthArray);
                }
                selectionSort(arr, lengthArray);
            

                /*
                * Bubble Sort
                * 
                   1 - average => sir generat random --> order = UNSORTED                                  
                   2 - worst   => sir descrescator   --> order = DESCENDING                         
                   3 - best    => sir crescator      --> order = ASCENDING    
                *
                * 
                */

                copyArray(arrCopy, arr, lengthArray);
                bubbleSort(arr, lengthArray);
            

 
                /*
                * Insertion Sort
                * 
                   1 - average => sir generat random  --> order = UNSORTED  
                   2 - worst   => sir descrescator    --> order = DESCENDING 
                   3 - best    => sir crescator       --> order = ASCENDING    
                *
                * 
                */

                copyArray(arrCopy, arr, lengthArray);
                classicInsertionSort(arr, lengthArray);

                copyArray(arrCopy, arr, lengthArray);
                binarySearchInsertionSort(arr, lengthArray);

            

        }
    }

 
        p.divideValues("selection-attr", NBS_TESTS);
        p.divideValues("selection-comp", NBS_TESTS);
        p.addSeries("selection", "selection-attr", "selection-comp");
      

        p.divideValues("bubble-attr", NBS_TESTS);
        p.divideValues("bubble-comp", NBS_TESTS);
        p.addSeries("bubble", "bubble-attr", "bubble-comp");
   

        p.divideValues("classicInsertion-attr", NBS_TESTS);
        p.divideValues("classicInsertion-comp", NBS_TESTS);
        p.addSeries("classicInsertion", "classicInsertion-attr", "classicInsertion-comp");
        

        p.divideValues("binarySearchInsertion-attr", NBS_TESTS);
        p.divideValues("binarySearchInsertion-comp", NBS_TESTS);
        p.addSeries("binarySearchInsertion", "binarySearchInsertion-attr", "binarySearchInsertion-comp");
      

        if (caseSelection == AVERAGE)
        {
            p.createGroup("attr-classicInsertion", "classicInsertion-attr");
            p.createGroup("attr-selection", "selection-attr");
            p.createGroup("comp-seletion", "selection-comp");
            p.createGroup("comp-binarySearchInsertion", "binarySearchInsertion-comp");
            p.createGroup("total-selection", "selection");
       
        }

        if (caseSelection == WORST)
        {
            p.createGroup("attr-classicInsetion", "classicInsertion-attr");
            p.createGroup("attr-selection", "selection-attr");
            p.createGroup("comp-bubble", "bubble-comp");
            p.createGroup("comp-selection", "selection-comp");
            p.createGroup("comp-binarySearchInsertion", "binarySearchInsertion-comp");
            p.createGroup("total-selection", "selection");
        }

        if (caseSelection == BEST)
        {
            p.createGroup("attr-selection", "selection-attr");
            p.createGroup("attr-classicInsertion", "classicInsertion-attr");
            p.createGroup("comp-bubble", "bubble-comp");
            p.createGroup("comp-binarySearchInsertion", "binarySearchInsertion-comp");
            p.createGroup("comp-classicInsertion", "classicInsertion-comp");
            p.createGroup("total-bubble", "bubble");
            p.createGroup("total-classicInsertion", "classicInsertion");
            p.createGroup("total-binarySearchInsertion", "binarySearchInsertion");

        }

        p.createGroup("attr", "selection-attr", "bubble-attr", "classicInsertion-attr", "binarySearchInsertion-attr");
        p.createGroup("comp", "selection-comp", "bubble-comp", "classicInsertion-comp", "binarySearchInsertion-comp");
        p.createGroup("total", "selection", "bubble", "classicInsertion", "binarySearchInsertion");    
    
}

void performance_cases()
{
    p.reset("average");
    performance(UNSORTED,AVERAGE);
    
    p.reset("worst");
    performance(DESCENDING,WORST);
        
    p.reset("best");
    performance(ASCENDING,BEST);
    

    p.showReport();
}

int main()
{
    demo();
    //performance_cases();


}