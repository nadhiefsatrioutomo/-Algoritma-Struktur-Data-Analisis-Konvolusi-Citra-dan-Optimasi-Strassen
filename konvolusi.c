#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>

// Fungsi Blur berdasarkan Listing 3 
void apply_blur(unsigned char* in, unsigned char* out, int w, int h, int c, int k_size) {
    int r = k_size / 2;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int ch = 0; ch < c; ch++) { // Loop komponen warna R, G, B 
                float sum = 0;
                int count = 0;
                for (int ky = -r; ky <= r; ky++) {
                    for (int kx = -r; kx <= r; kx++) {
                        int py = y + ky;
                        int px = x + kx;
                        // Boundary check (pengecekan batas gambar) 
                        if (px >= 0 && px < w && py >= 0 && py < h) {
                            sum += in[(py * w + px) * c + ch]; 
                            count++;
                        }
                    }
                }
                out[(y * w + x) * c + ch] = (unsigned char)(sum / count); 
            }
        }
    }
}

int main() {
    int w, h, c;
    // Ganti "input.png" dengan nama file gambar Anda
    unsigned char* img = stbi_load("input.png", &w, &h, &c, 0);
    if (!img) return 1;

    unsigned char* out = malloc(w * h * c);
    
    // Gunakan kernel size 15 untuk efek blur yang terlihat
    apply_blur(img, out, w, h, c, 15);

    stbi_write_png("output_blur.png", w, h, c, out, w * c);
    
    printf("Proses selesai. Cek output_blur.png\n");
    
    stbi_image_free(img);
    free(out);
    return 0;
}