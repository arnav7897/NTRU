# 🔐 NTRU Cryptosystem (Toy Implementation)

This is a simple and educational implementation of the NTRU public-key cryptosystem. NTRU is a lattice-based cryptosystem that is considered secure against quantum attacks, and is one of the candidates in the NIST Post-Quantum Cryptography standardization process.

## 📁 Project Structure
<pre><code>📁 ntru/ ├── 📁 src/ # Source code │ ├── poly.h # Polynomial structure and operations │ ├── poly.c # All custom polynomial functions │ ├── poly_inv.cpp # Uses NTL to compute inverse │ ├── LLL.cpp # LLL attack on toy NTRU │ └── ntru.c # Keygen, encryption, and decryption │ ├── 📁 assets/ # Input/output files │ ├── input.txt # Input for poly_inv.cpp │ ├── output.txt # Output of inverse from poly_inv.cpp │ ├── public.txt # Public key and encryption output │ └── secret.txt # Secret parameters: f (0:N), g (N:2N) │ ├── 📁 test_vectors/ # Sample inputs and test matrices │ └── ntru_matrix.txt # Matrix for LLL attack │ └── README.md # Project documentation </code></pre>
## 🚀 Features

- Polynomial arithmetic over rings
- Key generation using small ternary polynomials `f` and `g`
- Encryption and decryption functions
- Modular arithmetic with parameters `N`, `q`, and `p`
- Basic LLL integration for testing cryptanalysis (optional)
- Toy parameters: suitable for learning, not for production use

## ⚙️ Parameters Used

| Parameter | Description                   | Value (Example) |
|----------:|-------------------------------|----------------:|
| N         | Degree of the polynomials     | 7 / 11          |
| q         | Modulus for ring R_q          | 41 / 127        |
| p         | Small modulus (usually 3)     | 3               |
| D         | Number of non-zero coefficients in f, g | 2 / 3     |

> ⚠️ **Note**: These parameters are intentionally small for demonstration purposes and are **not secure** for real-world applications.

## 🧠 WORK FLOW OF CODE

1. **f_g_generation.c**  
   - Choose small ternary polynomials `f` and `g`
   - it is then saved in input.txt

2. **Inverse_Calculation**:  
   - poly_inv.cpp takes input from input.txt and calculate polynomial inverse of f mod Q and mod P     
   - result is stored in output.txt

3. **NTRU Schme**:  
   - ntru.c key gerneration ,encrytion , decrytion all three steps are done   
   - output is stored in secret.txt, public.txt , matrix.txt

## 🔧 Essential Libraries & Installation
   ### 🔹 For C-based Implementation

   Make sure you have a C compiler (`gcc`) and basic developer tools:

   #### ✅ Ubuntu / Debian (WSL or Linux):
      sudo apt update
      sudo apt install build-essential
   #### ✅ Install NTL (Ubuntu/Linux):
      sudo apt install libntl-dev


## 🛠️ Build & Run

  cd src  
  gcc f_g_generation.c -o f_g_generation
  ./f_g_generation  # making the f and g parameters  

  g++ -std=c++11 -o poly_inv poly_inv.cpp -lntl -lgmp
  ./poly_inv

  gcc ntru.c -o ntru
  ./ntru

  this below will give the pair of the best f,g after applying LLL to crack encryption
  g++ -std=c++11 -o lll lll.cpp -lntl -lgmp  
  ./lll