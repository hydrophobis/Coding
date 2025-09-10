#include <iostream>
#include <gmp.h>
#include <chrono>
#include <thread>
#include <cstring>
#include <limits>

// Optimized version with binary search - preallocates memory and uses more efficient operations
void fib_optimized(mpz_t fn, mpz_t fn1, unsigned long long n) {
    if (n == 0) {
        mpz_set_ui(fn, 0);
        mpz_set_ui(fn1, 1);
        return;
    }
    if (n == 1) {
        mpz_set_ui(fn, 1);
        mpz_set_ui(fn1, 1);
        return;
    }
    
    // Pre-allocate with estimated size to avoid reallocations
    size_t estimated_bits = (size_t)(n * 0.694); // log2(phi) ≈ 0.694
    
    mpz_t a, b, c, d, tmp1, tmp2;
    mpz_inits(a, b, c, d, tmp1, tmp2, nullptr);
    
    // Pre-allocate memory
    mpz_realloc2(a, estimated_bits);
    mpz_realloc2(b, estimated_bits);
    mpz_realloc2(c, estimated_bits);
    mpz_realloc2(d, estimated_bits);
    mpz_realloc2(tmp1, estimated_bits);
    mpz_realloc2(tmp2, estimated_bits);

    mpz_set_ui(a, 0);
    mpz_set_ui(b, 1);

    // Find the highest bit position more efficiently
    int start_bit = 63 - __builtin_clzll(n) - 1;
    
    for (int i = start_bit; i >= 0; --i) {
        // c = a * (2*b - a)
        mpz_mul_2exp(tmp1, b, 1);  // tmp1 = 2*b (bit shift is faster than multiplication)
        mpz_sub(tmp1, tmp1, a);    // tmp1 = 2*b - a
        mpz_mul(c, a, tmp1);       // c = a * (2*b - a)

        // d = a² + b²
        mpz_mul(tmp1, a, a);       // tmp1 = a²
        mpz_mul(tmp2, b, b);       // tmp2 = b²
        mpz_add(d, tmp1, tmp2);    // d = a² + b²

        if ((n >> i) & 1) {
            mpz_set(a, d);         // a = d
            mpz_add(b, c, d);      // b = c + d
        } else {
            mpz_set(a, c);         // a = c
            mpz_set(b, d);         // b = d
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
    unsigned long long high = 2000000000ULL;  // Increased search space
    unsigned long long best = 0;

    // CPU warmup - more aggressive
    auto warmup_start = std::chrono::high_resolution_clock::now();
    volatile int dummy = 0;
    while (std::chrono::high_resolution_clock::now() - warmup_start < std::chrono::milliseconds(500)) {
        for(int i = 0; i < 1000; ++i) dummy += i;
    }

    constexpr double target_time = 1.0;
    constexpr int num_samples = 5;  // More samples for better accuracy

    std::cout << "Searching for largest Fibonacci number computable in " << target_time << " second(s)...\n";

    while (low <= high) {
        unsigned long long mid = low + (high - low) / 2;

        double min_elapsed = std::numeric_limits<double>::max();
        
        // Take multiple measurements, use the minimum
        for (int sample = 0; sample < num_samples; ++sample) {
            auto start = std::chrono::high_resolution_clock::now();
            fib_optimized(fn, fn1, mid);
            auto end = std::chrono::high_resolution_clock::now();
            
            double elapsed = std::chrono::duration<double>(end - start).count();
            min_elapsed = std::min(min_elapsed, elapsed);
        }
        
        std::cout << "F(" << mid << "): " << min_elapsed << "s" << std::endl;
        
        if (min_elapsed <= target_time) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Final computation for display
    auto final_start = std::chrono::high_resolution_clock::now();
    fib_optimized(fn, fn1, best);
    auto final_end = std::chrono::high_resolution_clock::now();
    double final_time = std::chrono::duration<double>(final_end - final_start).count();
    
    std::cout << "\nLargest F(n) computable in " << target_time << " second(s): F(" << best << ")\n";
    std::cout << "Final computation time: " << final_time << "s\n";
    std::cout << "It has " << mpz_sizeinbase(fn, 10) << " decimal digits\n";
    
    char* str = mpz_get_str(nullptr, 10, fn);
    size_t len = strlen(str);
    if (len > 20) {
        std::cout << "Value: " << std::string(str, 10) << "..." 
                  << std::string(str + len - 10, 10) << std::endl;
    } else {
        std::cout << "Value: " << str << std::endl;
    }
    free(str);

    mpz_clears(fn, fn1, nullptr);
    return 0;
}
