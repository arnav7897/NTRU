#include <NTL/LLL.h>
#include <NTL/mat_ZZ.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using namespace NTL;

int main() {
    mat_ZZ B;
    ifstream in("../ntru_matrix.txt");

    if (!in.is_open()) {
        cerr << "Failed to open matrix file!" << endl;
        return 1;
    }

    vector<vector<long>> tempMatrix;
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<long> row;
        long val;
        while (iss >> val) row.push_back(val);
        if (!row.empty()) tempMatrix.push_back(row);
    }

    long rows = tempMatrix.size();
    long cols = tempMatrix[0].size();

    B.SetDims(rows, cols);
    for (long i = 0; i < rows; ++i)
        for (long j = 0; j < cols; ++j)
            B[i][j] = tempMatrix[i][j];

    cout << "Original Matrix:\n" << B << endl;

    // Apply LLL
    long rank = LLL_QP(B);

    cout << "\nLLL Reduced Matrix:\n" << B << endl;
    cout << "\nRank: " << rank << endl;

    // Find shortest vector that is NOT trivial (f or g all zero)
    long minIdx = -1;
    ZZ minNorm = ZZ(0);

    for (long i = 0; i < B.NumRows(); ++i) {
        ZZ norm;
        InnerProduct(norm, B[i], B[i]);

        // Split vector into f and g parts
        vec_ZZ candidate = B[i];
        long N = candidate.length() / 2;

        bool f_all_zero = true;
        bool g_all_zero = true;
        for (long j = 0; j < N; ++j) {
            if (candidate[j] != 0) f_all_zero = false;
        }
        for (long j = N; j < 2 * N; ++j) {
            if (candidate[j] != 0) g_all_zero = false;
        }

        if (f_all_zero || g_all_zero) {
            // Skip trivial solution
            continue;
        }

        if (minIdx == -1 || norm < minNorm) {
            minNorm = norm;
            minIdx = i;
        }
    }

    if (minIdx == -1) {
        cout << "No valid shortest vector found (non-trivial f and g)." << endl;
        return 1;
    }

    vec_ZZ candidate = B[minIdx];
    long N = candidate.length() / 2;

    cout << "\nShortest vector non trivial (potential f || g):\n" << candidate << endl;

    cout << "\nExtracted f: ";
    for (long i = 0; i < N; ++i) cout << candidate[i] << " ";
    cout << "\nExtracted g: ";
    for (long i = N; i < 2 * N; ++i) cout << candidate[i] << " ";
    cout << endl;

    return 0;
}
