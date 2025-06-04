# 🔐 NTRU Cryptosystem (Toy Implementation)

This is a simple and educational implementation of the NTRU public-key cryptosystem. NTRU is a lattice-based cryptosystem that is considered secure against quantum attacks, and is one of the candidates in the NIST Post-Quantum Cryptography standardization process.

## 📁 Project Structure

ntru/
├── src/ # Source code
│ ├── poly.h # Polynomial structure and operations 
| ├── poly.c # include all important self made functions 
| ├── poly_inv.cpp #by using NTL library computing inverse
| ├── LLL.cpp # Attacking this Toy version by LLL algorithm
│ └── ntru.c # Every main functions like key generation, encryption,decrypion is happening here
|
├──assets/ # all the .txt file are saved here
|   ├── input.txt  # serves as input for finding inverse through poly_inv.cpp using NTL lib 
|   ├── output.txt # contain output of inverse from poly_inv.cpp
|   ├── public.txt # contain public parameters like public key and encrytion
|   └── secret.txt # contain secret parameter in line just like a vector of dimention 2N parameters are f,g first N bits coeffecitents of f next N bit coeffitients of g  
|   
├── test_vectors/ # Sample inputs and expected outputs
├── ntru_matrix.txt # Matrix used for LLL or attack simulation
└── README.md # Project documentation

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

  #### cd src  
  #### gcc f_g_generation.c -o f_g_generation
  #### ./f_g_generation  # making the f and g parameters  

  #### g++ -std=c++11 -o poly_inv poly_inv.cpp -lntl -lgmp
  #### ./poly_inv

  #### gcc ntru.c -o ntru
  #### ./ntru

  #### g++ -std=c++11 -o lll lll.cpp -lntl -lgmp  
  this above will give the pair of the best f,g after applying LLL to crack encryption
  #### ./lll