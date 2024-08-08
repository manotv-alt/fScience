#include <iostream>
#include <stdlib.h>

using namespace std;

void swap_func(int *end1, int *end2){
    int swap;

    swap = *end1;
    *end1 = *end2;
    *end2 = swap;
}

int partition(int *vect, int beggin, int end){
    int reference = *(vect + end);
    int final_reference_index = beggin - 1;

    for(int run = beggin; run <= end - 1; run++){
        if(*(vect + run) <= reference){
            final_reference_index++;
            swap_func(vect + final_reference_index, vect + run);
        }
    }

    swap_func( vect + final_reference_index + 1, vect + end);

    return final_reference_index + 1;
}

void quicksort(int *vect, int beggin, int end){
    if(beggin < end){
        int q = partition(vect, beggin, end);
        quicksort(vect, beggin, q - 1);
        quicksort(vect,q + 1, end);
    }
}

int main(){

    int size;
    int *numbers_vect;
    cin >> size;

    numbers_vect = (int *) malloc(size*sizeof(int));
    for(int a = 0; a < size; a++){
        cin >> *(numbers_vect + a);
    }

    quicksort(numbers_vect, 0, size - 1);
    cout << endl;

    for(int a = 0; a < size; a++){
        cout << *(numbers_vect + a) << ' ';
    }
    
    cout << endl;

    free(numbers_vect);

    return 0;
}
