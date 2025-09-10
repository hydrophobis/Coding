#include <iostream>
#include <gmp.h>
#include <chrono>
#include <cstring>

// Highly optimized Fibonacci computation without binary search
void fib_fast(mpz_t result, unsigned long long n) {
    if (n == 0) {
        mpz_set_ui(result, 0);
        return;
    }
    if (n == 1 || n == 2) {
        mpz_set_ui(result, 1);
        return;
    }
    
    // Pre-allocate memory based on expected size
    // Fibonacci numbers grow as φ^n/√5, where φ ≈ 1.618
    // log2(F(n)) ≈ n * log2(φ) ≈ n * 0.694
    size_t estimated_bits = (size_t)(n * 0.694) + 100; // Extra padding
    
    mpz_t a, b, c, d, tmp1, tmp2;
    mpz_inits(a, b, c, d, tmp1, tmp2, nullptr);
    
    // Pre-allocate memory to avoid reallocations during computation
    mpz_realloc2(a, estimated_bits);
    mpz_realloc2(b, estimated_bits);
    mpz_realloc2(c, estimated_bits);
    mpz_realloc2(d, estimated_bits);
    mpz_realloc2(tmp1, estimated_bits);
    mpz_realloc2(tmp2, estimated_bits);

    mpz_set_ui(a, 0);  // F(0)
    mpz_set_ui(b, 1);  // F(1)

    // Find the position of the most significant bit
    int start_bit = 63 - __builtin_clzll(n) - 1;
    
    // Fast matrix exponentiation using binary method
    for (int i = start_bit; i >= 0; --i) {
        // Matrix squaring: [[a,b],[b,c]] -> [[c,d],[d,e]]
        // where c = a*(2*b-a), d = a² + b²
        
        mpz_mul_2exp(tmp1, b, 1);  // tmp1 = 2*b (left shift by 1 bit)
        mpz_sub(tmp1, tmp1, a);    // tmp1 = 2*b - a
        mpz_mul(c, a, tmp1);       // c = a * (2*b - a)

        mpz_mul(tmp1, a, a);       // tmp1 = a²
        mpz_mul(tmp2, b, b);       // tmp2 = b²
        mpz_add(d, tmp1, tmp2);    // d = a² + b²

        // Check if current bit of n is set
        if ((n >> i) & 1) {
            // Matrix multiplication by [[0,1],[1,1]]
            mpz_set(a, d);         // new a = d
            mpz_add(b, c, d);      // new b = c + d
        } else {
            mpz_set(a, c);         // new a = c
            mpz_set(b, d);         // new b = d
        }
    }

    mpz_set(result, a);  // F(n)
    mpz_clears(a, b, c, d, tmp1, tmp2, nullptr);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        std::cerr << "Computes the nth Fibonacci number F(n)" << std::endl;
        return 1;
    }

    unsigned long long n = std::strtoull(argv[1], nullptr, 10);
    
    mpz_t result;
    mpz_init(result);

    std::cout << "Computing F(" << n << ")..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    fib_fast(result, n);
    auto end = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double>(end - start).count();
    
    std::cout << "Computation time: " << elapsed << " seconds" << std::endl;
    std::cout << "F(" << n << ") has " << mpz_sizeinbase(result, 10) << " decimal digits" << std::endl;
    
    // Display the number (abbreviated if too long)
    char* str = mpz_get_str(nullptr, 10, result);
    size_t len = strlen(str);
    
    if (len <= 100) {
        std::cout << "F(" << n << ") = " << str << std::endl;
    } else {
        std::cout << "F(" << n << ") = " << std::string(str, 50) << "..." 
                  << std::string(str + len - 50, 50) << std::endl;
    }
    
    free(str);
    mpz_clear(result);
    
    return 0;
}
