#include <NTL/ZZ_pX.h>
#include <NTL/ZZ_p.h>
#include <iostream>
#include <fstream>
#include <vector>
extern "C" {
       #include "poly.h"
}


using namespace std;
using namespace NTL;

bool read_coefficients(const string& filename, vector<long>& coeffs) {
    ifstream inFile(filename); // reading the input file
    if (!inFile) {
        cerr << "Error opening input file: " << filename << endl; // error handling
        return false;
    }

    coeffs.clear();
    long val;
    while (inFile >> val) {  // puting the value of inFile in val
        cout << "Read coefficient: " << val << endl;  // Debug print
        coeffs.push_back(val);
        if ((long)coeffs.size() > N) {
            cerr << "Input file contains more than " << N << " coefficients.\n";
            return false;
        }
    }

    if ((long)coeffs.size() != N) {
        cerr << "Input file must contain exactly " << N << " coefficients.\n";
        return false;
    }

    return true;
}

void compute_and_append_inverse(long mod, const vector<long>& coeffs, const string& outputFilename) {
    ZZ_p::init(ZZ(mod));  // Set modulus

    ZZ_pX poly, inv, prod;
    poly.SetLength(N);

    for (long i = 0; i < N; ++i) {
        poly[i] = ZZ_p(coeffs[i]);
    }

    ZZ_pX modPoly;
    SetCoeff(modPoly, N, 1);
    SetCoeff(modPoly, 0, -1);

    ofstream outFile(outputFilename, ios::app);
    if (!outFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return;
    }

    long status = InvModStatus(inv, poly, modPoly);
    if (status != 0) {
        outFile << "Inverse does NOT exist modulo " << mod << "!\n\n";
        outFile.close();
        return;
    }

    outFile << "Inverse modulo " << mod << ": " << inv << "\n";
    outFile.close();
}

int main() {
    const string inputFilename = "../assets/input.txt";
    const string outputFilename = "../assets/output.txt";
    vector<long> coeffs;

    if (!read_coefficients(inputFilename, coeffs)) { // reading coff from input.txt
        return 1;
    }

    ofstream clearFile(outputFilename); // writing in file by clearing it first
    clearFile.close(); // file closing

    compute_and_append_inverse(Q, coeffs, outputFilename);
    compute_and_append_inverse(P, coeffs, outputFilename);

    cout << "Done! Results appended to " << outputFilename << endl;
    return 0;
}
