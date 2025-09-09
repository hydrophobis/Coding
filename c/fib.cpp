#include <iostream>
#include <gmp.h>
#include <chrono>
#include <thread>

void fib_fast_doubling(mpz_t fn, mpz_t fn1, unsigned long long n) {
    mpz_t a, b, c, d, tmp1, tmp2;
    mpz_inits(a, b, c, d, tmp1, tmp2, nullptr);

    mpz_set_ui(a, 0);
    mpz_set_ui(b, 1);

    for (int i = 63 - __builtin_clzll(n); i >= 0; --i) {
        mpz_mul_ui(tmp1, b, 2);
        mpz_sub(tmp1, tmp1, a);
        mpz_mul(c, a, tmp1);

        mpz_mul(tmp1, a, a);
        mpz_mul(tmp2, b, b);
        mpz_add(d, tmp1, tmp2);

        if ((n >> i) & 1) {
            mpz_set(a, d);
            mpz_add(b, c, d);
        } else {
            mpz_set(a, c);
            mpz_set(b, d);
        }
    }

    mpz_set(fn, a);
    mpz_set(fn1, b);

    mpz_clears(a, b, c, d, tmp1, tmp2, nullptr);
}

int main() {
    mpz_t fn, fn1;
    mpz_inits(fn, fn1, nullptr);

    unsigned long long low = 0;
    unsigned long long high = 200'000'000; // barely over 1 second
    unsigned long long best = 0;

    // Get CPU running on boost
    auto warmup_start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - warmup_start < std::chrono::milliseconds(200)) {
        asm volatile("" ::: "memory"); // prevent the compiler from optimizing away the loop
    }
    auto spin_start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - spin_start < std::chrono::milliseconds(100)) { }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));


    
    #define time 1.0

    while (low <= high) {
        unsigned long long mid = low + (high - low) / 2;

        auto start = std::chrono::high_resolution_clock::now();
        fib_fast_doubling(fn, fn1, mid);
        auto end = std::chrono::high_resolution_clock::now();

        double elapsed = std::chrono::duration<double>(end - start).count();
        std::cout << mid << ": " << elapsed << std::endl;
        if (elapsed <= time) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    std::cout << "Largest F(n) computable in " << time << " second(s): F(" << best << ")\n";
    std::cout << "It has " << mpz_sizeinbase(fn, 10) << " decimal digits\n";

    mpz_clears(fn, fn1, nullptr);
    return 0;
}
