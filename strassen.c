#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fungsi alokasi memori dinamis 
int** alloc_m(int n) {
    int** m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        m[i] = (int*)malloc(n * sizeof(int));
    }
    return m;
}

// Fungsi membebaskan memori untuk mencegah Memory Leak 
void free_m(int** m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

// Inisialisasi matriks dengan angka random 0-9 
void init_rand(int** m, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) 
            m[i][j] = rand() % 10;
}

// Fungsi pembantu: Penjumlahan Matriks
void add(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

// Fungsi pembantu: Pengurangan Matriks
void sub(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Fungsi Rekursif Strassen
void strassen(int** A, int** B, int** C, int n) {
    // Base case: jika ukuran 1x1, lakukan perkalian biasa
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int new_n = n / 2;
    int** a00 = alloc_m(new_n); int** a01 = alloc_m(new_n);
    int** a10 = alloc_m(new_n); int** a11 = alloc_m(new_n);
    int** b00 = alloc_m(new_n); int** b01 = alloc_m(new_n);
    int** b10 = alloc_m(new_n); int** b11 = alloc_m(new_n);
    
    // Fase 1: Divide (Memecah matriks menjadi 4 bagian) 
    for (int i = 0; i < new_n; i++) {
        for (int j = 0; j < new_n; j++) {
            a00[i][j] = A[i][j];
            a01[i][j] = A[i][j + new_n];
            a10[i][j] = A[i + new_n][j];
            a11[i][j] = A[i + new_n][j + new_n];

            b00[i][j] = B[i][j];
            b01[i][j] = B[i][j + new_n];
            b10[i][j] = B[i + new_n][j];
            b11[i][j] = B[i + new_n][j + new_n];
        }
    }

    // Variabel bantu untuk menyimpan hasil penjumlahan/pengurangan
    int** resA = alloc_m(new_n); int** resB = alloc_m(new_n);
    
    // Fase 2: Rekursi (Menghitung M1 - M7) 
    int** m1 = alloc_m(new_n); int** m2 = alloc_m(new_n);
    int** m3 = alloc_m(new_n); int** m4 = alloc_m(new_n);
    int** m5 = alloc_m(new_n); int** m6 = alloc_m(new_n);
    int** m7 = alloc_m(new_n);

    // M1 = (a00 + a11) * (b00 + b11)
    add(a00, a11, resA, new_n); add(b00, b11, resB, new_n);
    strassen(resA, resB, m1, new_n);

    // M2 = (a10 + a11) * b00
    add(a10, a11, resA, new_n);
    strassen(resA, b00, m2, new_n);

    // M3 = a00 * (b01 - b11)
    sub(b01, b11, resB, new_n);
    strassen(a00, resB, m3, new_n);

    // M4 = a11 * (b10 - b00)
    sub(b10, b00, resB, new_n);
    strassen(a11, resB, m4, new_n);

    // M5 = (a00 + a01) * b11
    add(a00, a01, resA, new_n);
    strassen(resA, b11, m5, new_n);

    // M6 = (a10 - a00) * (b00 + b01)
    sub(a10, a00, resA, new_n); add(b00, b01, resB, new_n);
    strassen(resA, resB, m6, new_n);

    // M7 = (a01 - a11) * (b10 + b11)
    sub(a01, a11, resA, new_n); add(b10, b11, resB, new_n);
    strassen(resA, resB, m7, new_n);

    // Fase 3: Conquer (Menggabungkan hasil menjadi matriks C) 
    for (int i = 0; i < new_n; i++) {
        for (int j = 0; j < new_n; j++) {
            // C00 = m1 + m4 - m5 + m7
            C[i][j] = m1[i][j] + m4[i][j] - m5[i][j] + m7[i][j];
            // C01 = m3 + m5
            C[i][j + new_n] = m3[i][j] + m5[i][j];
            // C10 = m2 + m4
            C[i + new_n][j] = m2[i][j] + m4[i][j];
            // C11 = m1 - m2 + m3 + m6
            C[i + new_n][j + new_n] = m1[i][j] - m2[i][j] + m3[i][j] + m6[i][j];
        }
    }

    // Bebaskan semua memori sementara
    free_m(a00, new_n); free_m(a01, new_n); free_m(a10, new_n); free_m(a11, new_n);
    free_m(b00, new_n); free_m(b01, new_n); free_m(b10, new_n); free_m(b11, new_n);
    free_m(resA, new_n); free_m(resB, new_n);
    free_m(m1, new_n); free_m(m2, new_n); free_m(m3, new_n);
    free_m(m4, new_n); free_m(m5, new_n); free_m(m6, new_n); free_m(m7, new_n);
}

int main() {
    int n = 4; // Ukuran matriks (harus pangkat 2)
    srand(time(NULL));

    int** A = alloc_m(n);
    int** B = alloc_m(n);
    int** C = alloc_m(n);

    init_rand(A, n);
    init_rand(B, n);

    printf("Matriks A:\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) printf("%d ", A[i][j]);
        printf("\n");
    }

    strassen(A, B, C, n);

    printf("\nHasil Strassen %dx%d:\n", n, n);
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) printf("%d ", C[i][j]);
        printf("\n");
    }

    free_m(A, n);
    free_m(B, n);
    free_m(C, n);
    return 0;
}