# 🔐 NTRU Cryptosystem (Toy Implementation)

This is a simple and educational implementation of the NTRU public-key cryptosystem. NTRU is a lattice-based cryptosystem that is considered secure against quantum attacks and is one of the candidates in the NIST Post-Quantum Cryptography standardization process.

---

## 📁 Project Structure
```
📁 ntru/
├── 📁 src/ # Source code
│ ├── poly.h # Polynomial structure and operations
│ ├── poly.c # All custom polynomial functions
│ ├── poly_inv.cpp # Uses NTL to compute inverse
│ ├── LLL.cpp # LLL attack on toy NTRU
│ └── ntru.c # Keygen, encryption, and decryption
│
├── 📁 assets/ # Input/output files
│ ├── input.txt # Input for poly_inv.cpp
│ ├── output.txt # Output of inverse from poly_inv.cpp
│ ├── public.txt # Public key and encryption output
│ └── secret.txt # Secret parameters (f and g as 2N vector)
│
├── ntru_matrix.txt # Matrix for LLL attack
│
└── README.md # Project documentation
```
---

## 🚀 Features

- Polynomial arithmetic over rings
- Key generation using small ternary polynomials `f` and `g`
- Encryption and decryption functions
- Modular arithmetic with parameters `N`, `q`, and `p`
- Optional LLL-based cryptanalysis attack
- **Toy parameters**: suitable for learning, **not** secure for real use

---

## ⚙️ Parameters Used

| Parameter | Description                            | Example Value |
|-----------|----------------------------------------|---------------|
| `N`       | Degree of the polynomials              | 503        |
| `q`       | Large modulus for ring R_q             | 2053      |
| `p`       | Small modulus (usually small prime)    | 3             |
| `D`       | Non-zero coefficients in `f`, `g`      | 143        |

> ⚠️ **Note:** These are toy parameters for demonstration only — do not use in production.

---

## 🧠 Code Workflow

1. **`f_g_generation.c`**  
   - Randomly generates small ternary polynomials `f` and `g`  
   - Saves coefficients to `input.txt`

2. **`poly_inv.cpp` (Inverse Calculation)**  
   - Uses NTL to compute inverses of `f` mod `p` and `q`  
   - Results stored in `output.txt`

3. **`ntru.c` (Core NTRU Scheme)**  
   - Performs key generation, encryption, and decryption  
   - Output saved in `secret.txt`, `public.txt`, and matrix files

4. **`LLL.cpp` (LLL Attack)**  
   - Demonstrates how toy NTRU can be broken using lattice reduction  
   - Suggests best `(f, g)` candidates from `ntru_matrix.txt`

---

## 🔧 Requirements & Installation

### 🔹 C/C++ Dependencies

Make sure you have the following installed:

#### ✅ On Ubuntu / Debian / WSL:

```bash
sudo apt update
sudo apt install build-essential
sudo apt install libntl-dev
```
## 🛠️ Build & Run
cd src

#### 1. Generate f and g
```
gcc f_g_generation.c -o f_g_generation
./f_g_generation
```
#### 2. Compute inverses using NTL
```
g++ -std=c++11 -o poly_inv poly_inv.cpp -lntl -lgmp
./poly_inv
```
#### 3. Run NTRU scheme: keygen, encrypt, decrypt
```
gcc ntru.c -o ntru
./ntru
```
#### 4. Run LLL attack
```
g++ -std=c++11 -o lll LLL.cpp -lntl -lgmp
./lll
```
