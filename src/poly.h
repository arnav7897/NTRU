#ifndef POLY_H
#define POLY_H

// ===============================
// poly.h - Header for NTRU Polynomial Operations
// ===============================
//
// This file defines the core data structures and function prototypes
// used for implementing a toy version of the NTRU public key cryptosystem.
//
// ===============================

/*
NTRU Parameter Constraints:
---------------------------
- N: Degree of the polynomial ring (polynomials are modulo x^N - 1)
- Q: Large modulus used for the ring R_Q = Z_Q[x]/(x^N - 1)
- P: Small modulus used for the message space
- D: Determines the number of +1s and -1s in ternary polynomials
      (important for private keys in NTRU)

Parameter Requirement: 
    Q > (6D + 1) * P   and   gcd(P, Q) = 1
*/

#define N 7       // Degree of polynomials
#define Q 41      // Large modulus (for ciphertext space)
#define P 3       // Small modulus (for plaintext space)
#define D 2       // Number of +1s and -1s in ternary polynomials

// ===============================
// Polynomial Data Structure
// ===============================
typedef struct {
    int coff[N];  // Array of coefficients (degree N-1 polynomial)
} poly;

// ===============================
// Function: ternary
// -------------------------------
// Generates a ternary polynomial (coefficients in {-1, 0, 1})
// - If mode = 1: generates a T(D+1, D) polynomial (D+1 positive, D negative coefficients)
// - If mode = 0: generates a balanced T(D, D) polynomial
//
// Parameters:
//  1. res  - pointer to the result polynomial
// 2.     mode - 1 for T(D+1, D), 0 for T(D, D)
// ===============================
void tertnery(poly* res, int mode);

// ===============================
// Function: print_poly
// -------------------------------
// Utility function to print the polynomial coefficients in a readable format.
//
// Parameters:
//   name       - a label to prefix the output
//   polynomial - pointer to the polynomial to print
//
// ===============================
void print_poly(char name[], poly *polynomial);

// ===============================
// Function: poly_add_mod
// -------------------------------
// Adds two polynomials modulo a given modulus.
// Performs coefficient-wise modular addition: res = (x + y) mod 'mod'
//
// Parameters:
//  1. res  - result polynomial
//  2. x    - first operand polynomial
//  3. y    - second operand polynomial
// 4.    mod  - modulus (Q or P depending on context)
// ===============================
void poly_add_mod(poly* res, poly* x, poly* y, int mod);

// ===============================
// Function: poly_multiply_mod
// -------------------------------
// Multiplies two polynomials modulo (x^N - 1) and a modulus.
// Uses convolution-like logic and reduces coefficients modulo 'mod'.
//
// Parameters:
//   1 .res - result polynomial
//   2. a   - first operand polynomial
//   3. b   - second operand polynomial
//  4.   mod - modulus (Q or P depending on context)
// ===============================
void poly_multiply_mod(poly* res, poly* a, poly* b, int mod);

// ===============================
// Function: key_generation
// -------------------------------
// Generates public and private key polynomials for NTRU encryption.
// - Generates f and g from ternary distributions
// - Computes f^-1 mod Q (Fq) and public key h = Fq * g mod Q
//
// Parameters:
//   1. result - stores the public key h
//   2. Fq     - input the inverse of f mod Q
//   3.    g      - input the g polynomial used
// ===============================
void key_generation(poly* result, poly* Fq, poly* g);

// ===============================
// Function: ntru_encryption
// -------------------------------
// Encrypts a random message polynomial using the public key.
// Follows: c = p*h + e mod Q (simplified for toy version)
//
// Parameters:
//  1. result                - stores the resulting ciphertext
//  2. message               - input the message(message polynomial coff must be between (-p/2,p/2))   
//  3.     public_key_polynomial - public key polynomial h
// ===============================
void ntru_encryption(poly* result,poly *message,poly* public_key_polynomial);

// ===============================
// Function: ntru_decryption
// -------------------------------
// Decrypts a ciphertext polynomial to recover the original message.
// Steps:
//   1. a = f * c mod Q
//   2. b = a mod P
//   3. m = Fp * b mod P
//
// Parameters:
//  1. result - stores the decrypted message
//  2. cipher - input ciphertext
//  3. Fp     - inverse of f modulo P
//  4.    f      - private key polynomial
// ===============================
void ntru_decryption(poly* result, poly* cipher, poly* Fp, poly* f);

#endif // POLY_H
