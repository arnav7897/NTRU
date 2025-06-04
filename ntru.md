# 🔐 NTRU Cryptosystem & LLL Attack — Technical Overview

This document explains the fundamentals of the **NTRU cryptosystem**, its **real-world implementation strategy**, and how **LLL lattice reduction** can be used to attack toy or poorly parameterized versions.

---

## 📌 What is NTRU?

NTRU (Nth degree truncated polynomial ring unit) is a lattice-based public-key cryptosystem. Unlike RSA or ECC, which rely on number-theoretic problems (like factoring), NTRU relies on the **hardness of certain lattice problems**, especially **finding short vectors in structured lattices**.

NTRU is fast and efficient, making it well-suited for post-quantum cryptography.

---

## 🔧 Real Implementation Components

In real-world cryptographic use, NTRU includes:

### 🔹 Parameters
- **N**: Degree of the ring polynomial (typically 509, 677, or 821)
- **q**: Large modulus (prime), e.g., 2048 or 12289
- **p**: Small modulus, typically 3
- **f, g**: Small, ternary polynomials with coefficients in {−1, 0, 1}
- **R = Z[x]/(x^N − 1)**

### 🔹 Key Generation
1. Choose small polynomials `f`, `g`

2. Compute inverses:
   - $f_p^{-1} \mod p$
   - $f_q^{-1} \mod q$

3. Compute the public key:
   - $h = p \cdot f_q^{-1} \cdot g \mod q$

4. Private key:
   - $(f,\ f_p^{-1},\ f_q^{-1})$
### 🔹 Encryption
To encrypt message polynomial `m`:
1. Choose random small polynomial `r`
2. Compute:
   \[
   e = r \cdot h + m \mod q
   \]

### 🔹 Decryption
1. Compute:
   \[
   a = f \cdot e \mod q
   \]
2. Lift `a` to centered mod `q`, then reduce mod `p`
3. Recover:
   \[
   m = f_p^{-1} \cdot a \mod p
   \]

---

## 🔐 Security Basis

NTRU's security is based on:
- The **hardness of finding short vectors in lattices**
- The **non-commutative ring structure** preventing certain algebraic attacks

Proper parameter selection (e.g., large `N`, carefully chosen `f`, `g`, `q`) is crucial to maintaining security.

---

## ⚠️ LLL Attack on Toy Implementations

LLL (Lenstra–Lenstra–Lovász) is a polynomial-time algorithm that finds **short, nearly orthogonal** vectors in integer lattices.

### ⚔️ Attack Strategy

1. Given a public key `h`, build a **convolution matrix** (a lattice) that includes `h`.
2. Use **LLL** to reduce the lattice basis and extract a **short vector**.
3. If toy parameters are used (small `N`, small `q`), LLL might recover `f` or `g` (private key).

### 📉 Why It Works on Toy Versions
- With small `N` (e.g., 7 or 11), and small modulus `q`, the short vector representing `f`, `g` becomes **vulnerable**.
- LLL works best when lattice dimension is low and the gap between shortest and average vector is narrow.

### ✅ How We Demonstrate This

In the toy version:
- Construct matrix `B` from public key `h` and `q`
- Call LLL algorithm on `B`
- Analyze if output matches a valid `(f, g)` pair

---

## 🧪 Example: Toy NTRU Setup

| Parameter | Value |
|----------:|-------|
| N         | 7     |
| q         | 41    |
| p         | 3     |
| D         | 2     |

- `f`, `g` are ternary
- `h = p*f_q⁻¹ * g mod q`
- Public key saved
- Attack using LLL on matrix derived from `h`

This is intentionally insecure, but ideal for **demonstrating attacks**.

---

## 📚 References

- [NTRUEncrypt Specification](https://datatracker.ietf.org/doc/html/draft-ietf-lamps-ntru-00)
- [NIST PQC Round 3 – NTRU](https://csrc.nist.gov/Projects/post-quantum-cryptography/round-3-submissions)
- [LLL Algorithm Overview – Wikipedia](https://en.wikipedia.org/wiki/LLL_algorithm)
- Victor Shoup's NTL Library: https://libntl.org/

---

## 📌 Summary

| Concept           | Secure NTRU | Toy NTRU |
|------------------|-------------|----------|
| Polynomial Degree | ≥ 509       | 7–11     |
| Lattice Dimension | High        | Low      |
| LLL Resistance    | Strong      | Weak     |
| Real Use          | ✅           | ❌        |
| Educational Use   | ✅           | ✅        |

---

## ✍️ Author

*This document was created as part of a learning project to understand how NTRU works and how lattice attacks like LLL can be used to break improperly configured implementations.*

