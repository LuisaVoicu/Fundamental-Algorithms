#include <stdio.h>
// e suficient sa am parcurgerea in postOrdine


int structuraABCcuPostOrdine(int* post, int left, int right, int parent, int* position)
{
    if (left <= right)
    {

        printf("%d este copilul lui %d\n", post[right], parent);
        int j = right;
        while (post[j - 1] >= post[right])
        {
            j--;
        }
        // de la left la j-1 subarbore stang pt nodul post(right) si de la j la right-1 subarbore drept pt nodul post(right)
        int rightChild, leftChild;
        //merg pe subarborele drept deci de la j la right - 1 
        leftChild = structuraABCcuPostOrdine(post, left, j - 1, post[right], position);
        rightChild = structuraABCcuPostOrdine(post, j, right - 1, post[right], position);


        // am terminat cu subarborele drept.. ma duc pe cel din stanga 

        printf("%d(parent) -> %d (left child) , %d (right child)\n", post[right], leftChild, rightChild);
        return post[right];
    }
    else
    { 

        return -1;
    }
}


int reStructuraABC(int* post, int left, int right, int* position)
{
    if (left <= right&& left >=0)
    {

        printf("%d %d ----- %d %d\n", left , right, post[left],post[right]);

        if (post[left] > post[right]) {
            return left;
        }
        
        int posRight = reStructuraABC(post, left+1, right, position);
        int posleft = reStructuraABC(post, posRight, right-1, position);
        printf("%d este parintele lui %d\n", post[right], post[left]);

        /*int rightRecursive = -1, leftRecursive = -1;
        if (post[left] < post[*position])
        {

            leftRecursive = reStructuraABC(post, left + 1, *position, position);
            (*position)--;

        }
        else
        {
            printf("%d este parintele lui %d %d\n", post[*position], post[left-1], *position);
           // printf("arb drept pt %d incepe de la %d-->%d\n", post[right], *position, post[*position]);
        }
        printf("%d %d ----- atf\n", left , right);

        printf("\n------------ %d %d\n", *position, right);
        if (post[*position] < post[right])
        {
            printf("hei");
            rightRecursive = reStructuraABC(post, *position , right, position);
            (*position)++;
        }
        else
        {
            printf("%d este parintele lui %d %d\n", post[*position], post[left - 1], *position);

        }*/
       
    }
    return -1;
}


int main()
{
    //int post[] = {1,3,2,8,5,11,12,13,10,14 }; 
    int post[] = { 4,8,13,6,15,31,20,17,14 };
    int n = sizeof(post) / sizeof(int);
    int position = n - 1;
    structuraABCcuPostOrdine(post, 0, n-1,-1,&position);
    reStructuraABC(post, 0, n-1, &position);

}