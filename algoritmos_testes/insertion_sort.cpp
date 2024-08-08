#include <iostream>
#include <vector>
using namespace std;

void insertion_sort(vector<int> &vet, int tamanho) {
    
    for(int i = 1; i < tamanho; i++) {
        int j = i;
        while(j > 0 && vet[j] < vet[j-1]) {
            int ax = vet[j];
            vet[j] = vet[j-1];
            vet[j-1] = ax;
            j--;
        }
    }
    
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int qtd;
    cin >> qtd;
    vector<int> vet(qtd);
    for(int c = 0; c < qtd; c++){
        cin >> vet[c];
    }

    insertion_sort(vet, vet.size());
    /*
    for(int c = 0; c < qtd; c++){
        cout << vet[c] << "\n";
    }
    */
    return 0;
}