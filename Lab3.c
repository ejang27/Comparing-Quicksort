

//Title: Lab3.c
//Written by: Eunsoo Jang
//Reference:https://codeblab.com/wp-content/uploads/2009/09/DualPivotQuicksort.pdf
//Dual-Pivot Quicksort algorithm by Vladimir Yaroslavskiy
//Date: Feb 11, 2020


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 50000000
#define CUTOFF 25
int A[MAX_SIZE];

int compare(const void*, const void*);
void quicksort(int[], int, int);
void quickSort(int[], int, int);
void insertionSort(int[], int, int);
void hybridSort(int[], int, int, int);
void dualpivotQSort(int[], int, int, int);
void mySwap(int*, int*);
int randomInt(int, int);
void fisherShuffle(int[]);


int main(void){
  for(int i =0; i < MAX_SIZE; i++){
    A[i] = i;
  }

//running the different quicksorts from 5-50million sized arrays
  printf("Quicksort:\n");
  for (int i = 5000000; i <= MAX_SIZE; i+=5000000){
    fisherShuffle(A);
    long start = clock();
    quicksort(A, 0, i-1);
    double duration = ((clock() - (float)start) / CLOCKS_PER_SEC);
    printf("%f\n", duration);
  }

  printf("hybridSort:\n");
  for (int i = 5000000; i <= MAX_SIZE; i+=5000000){
    fisherShuffle(A);
    long start = clock();
    hybridSort(A, 0, i-1,i);
    double duration = ((clock() - (float)start) / CLOCKS_PER_SEC);
    printf("%f\n", duration);
  }

  printf("Built in Sort:\n");
  for (int i = 5000000; i <= MAX_SIZE; i+=5000000){
    fisherShuffle(A);
    long start = clock();
    qsort(A, i, sizeof(int), compare);
    double duration = ((clock() - (float)start) / CLOCKS_PER_SEC);
    printf("%f\n", duration);
  }

  printf("dualpivotSort:\n");
  for (int i = 5000000; i <= MAX_SIZE; i+=5000000){
    fisherShuffle(A);
    long start = clock();
    dualpivotQSort(A, 0, i-1, 3);
    double duration = ((clock() - (float)start) / CLOCKS_PER_SEC);
    printf("%f\n", duration);
  }

  return 0;
}

//compare: helper function to use in the built in qsort
int compare(const void* a, const void* b){
  return ( *(int*)a - *(int*)b );
}

//quicksort: quick sort function using Bentley's quicksort algorithm
void quicksort(int A[], int left, int right){
  if(left>=right){
    return;
  }
  int random = randomInt(left,right);
  mySwap(A+left, A+random);
  int m = left;
  for (int i = left+1; i <= right;i++){
    if(A[i] < A[left]){
      mySwap(A+(++m), A+i);
    }
  }
  mySwap(A+left, A+m);
  quicksort(A, left, m-1);
  quicksort(A,m+1, right);
}

//quickSort: quicksort modified to be used in the Hybrid sort
void quickSort(int A[], int left, int right){
  if(right-left < CUTOFF){
    return;
  }
  int random = randomInt(left,right);
  mySwap(A+left, A+random);

  int m = left;

  for (int i = left+1; i <= right;i++){
    if(A[i] < A[left]){
      mySwap(A+(++m), A+i);
    }
  }
  mySwap(A+left, A+m);
  quickSort(A, left, m-1);
  quickSort(A,m+1, right);
}

//insertionSort: standard insertion sort function
void insertionSort(int A[], int left, int right){
  for(int i = left+1; i <=right-1; i++){
    int key = A[i];
    int j = i-1;
    while(j>=0 && A[j] > key){
      A[j+1] = A[j];
      j--;
    }
    A[j+1]=key;
  }
}

//hybridSort: hybrid sort that uses quick sort and insertion sort
void hybridSort(int A[], int left, int right, int size){
  quickSort(A, left, right);
  insertionSort(A,0,size);
}

//Reference: https://codeblab.com/wp-content/uploads/2009/09/DualPivotQuicksort.pdf
//Dual-Pivot Quicksort algorithm by Vladimir Yaroslavskiy
void dualpivotQSort(int A[], int left, int right, int divide){
    int size = right-left;
    if(size<27){
      insertionSort(A, left, right+1);
      return;
    }
    int third = size/divide;

    int med_1 = left + third;
    int med_2 = right - third;

    if(med_1 <= left){
        med_1 = left + 1;
    }
    if(med_2 >= right){
        med_2 = right -1;
    }
    if(A[med_1] < A[med_2]){
        mySwap(A+med_1, A+left);
        mySwap(A+med_2, A+right);
    }
    else{
        mySwap(A+med_1, A+right);
        mySwap(A+med_2, A+left);
    }
    int pivot1 = A[left];
    int pivot2 = A[right];

    int less = left + 1;
    int great = right - 1;

     for (int k = less; k <= great; k++) {
        if (A[k] < pivot1) {
            mySwap(A+k, A+(less++));
        }
        else if (A[k] > pivot2) {
            while (k < great && A[great] > pivot2) {
                 great--;
            }
            mySwap(A+k, A+(great--));
            if (A[k] < pivot1) {
                mySwap(A+k,A+(less++));
            }
        }
     }

     int dist = great - less;
     if (dist < 13) {
        divide++;
     }
     mySwap(A+(less - 1), A+left);
     mySwap(A+(great + 1), A+right);

     dualpivotQSort(A, left, less - 2, divide);
     dualpivotQSort(A, great + 2, right, divide);

     if (dist > size - 13 && pivot1 != pivot2) {
        for (int k = less; k <= great; k++) {
            if (A[k] == pivot1) {
                mySwap(A+k, A+(less++));
            }
            else if (A[k] == pivot2) {
                mySwap(A+k, A+(great--));
                if (A[k] == pivot1) {
                    mySwap(A+k, A+(less++));
                }
            }
        }
     }
     if (pivot1 < pivot2) {
        dualpivotQSort(A, less, great, divide);
     }

}

//mySwap: function that swaps two values
void mySwap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

//randomInt: function that returns a random int with the given range
int randomInt(int lower, int upper){
  return lower + (rand() % (upper+(-lower)));
}

//fisherShuffle: shuffles the given array
void fisherShuffle(int A[]){
  srand(time(NULL));
  for(int i =0; i < MAX_SIZE-2; i++){
    int j = randomInt(i, MAX_SIZE);
    mySwap(A+i, A+j);
  }
}
