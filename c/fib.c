#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main() {
    mpz_t a, b, tmp;
    mpz_init_set_ui(a, 0);
    mpz_init_set_ui(b, 1);
    mpz_init(tmp);

    unsigned long long n = 1;

    clock_t start = clock();
    clock_t now = clock();
    double elapsed = 0.0;

    while (1) {
        mpz_add(tmp, a, b);  // tmp = a + b
        mpz_set(a, b);       // a = b
        mpz_set(b, tmp);     // b = tmp
        n++;

        now = clock();
        elapsed = (double)(now - start) / CLOCKS_PER_SEC;
        if (elapsed >= 1.0) break; // stop after ~1 second
    }

    size_t digits = mpz_sizeinbase(b, 10);

    printf("Computed up to F(%llu) in 1 second\n", n);
    printf("F(%llu) has %zu decimal digits\n", n, digits);

    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(tmp);

    return 0;
}
