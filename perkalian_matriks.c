#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Modul 2: Alokasi Memori Dinamis 
int** alloc_m(int n) {
    int** m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        m[i] = (int*)malloc(n * sizeof(int));
    }
    return m;
}

// Membebaskan memori untuk mencegah Memory Leak
void free_m(int** m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

// Inisialisasi matriks dengan angka random 0-9
void init_rand(int** m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
        }
    }
}

// Modul 1: Perkalian Matriks Standar O(n^3) 
void multiply_standard(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Modul 2: Logika Strassen 2x2 
void strassen_2x2(int** A, int** B, int** C) {
    // 7 Perkalian Strassen [cite: 78]
    int m1 = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
    int m2 = (A[1][0] + A[1][1]) * (B[0][0]);
    int m3 = A[0][0] * (B[0][1] - B[1][1]);
    int m4 = A[1][1] * (B[1][0] - B[0][0]);
    int m5 = (A[0][0] + A[0][1]) * (B[1][1]);
    int m6 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
    int m7 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);

    // Kombinasi menjadi hasil akhir 
    C[0][0] = m1 + m4 - m5 + m7;
    C[0][1] = m3 + m5;
    C[1][0] = m2 + m4;
    C[1][1] = m1 - m2 + m3 + m6;
}

int main() {
    srand(time(NULL));
    int n = 2; // Ukuran matriks (bisa diubah sesuai instruksi N=512/1024)

    int** A = alloc_m(n);
    int** B = alloc_m(n);
    int** C = alloc_m(n);

    init_rand(A, n);
    init_rand(B, n);

    printf("Matriks A (2x2):\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++) printf("%d ", A[i][j]);
        printf("\n");
    }

    // Eksekusi Strassen
    strassen_2x2(A, B, C);

    printf("\nHasil Perkalian Strassen:\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) printf("%d ", C[i][j]);
        printf("\n");
    }

    // Jangan lupa bebaskan memori! 
    free_m(A, n);
    free_m(B, n);
    free_m(C, n);

    return 0;
}