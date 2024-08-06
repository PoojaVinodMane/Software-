#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <pthread.h>
#include <stdexcept>

using namespace std;

// Function to set thread affinity
void SetThreadAffinity(thread& th, int core) {
    pthread_t native_handle = th.native_handle();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    int result = pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        throw runtime_error("Failed to set thread affinity.");
    }
}

// Function to perform matrix multiplication in a chunk
void MultiplicationChunk(const vector<vector<int>>& A, vector<vector<int>>& C, int start, int end) {
    int N = A.size();
    for (int i = start; i < end; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

// Function to perform matrix multiplication in a mixed fashion
void MultiplicationMixed(const vector<vector<int>>& A, vector<vector<int>>& C, int threadId, int K) {
    int N = A.size();
    for (int i = threadId; i < N; i += K) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

int main() {
    string inputFile = "inp.txt";

    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    int sizeN, threadsK, coresC, boundedThreadsBT;

    inFile >> sizeN >> threadsK >> coresC >> boundedThreadsBT;
    if (sizeN <= 0 || threadsK <= 0 || coresC <= 0 || boundedThreadsBT < 0) {
        cerr << "Invalid input values." << endl;
        return 1;
    }

    vector<vector<int>> matrixA(sizeN, vector<int>(sizeN, 0));
    for (int i = 0; i < sizeN; ++i) {
        for (int j = 0; j < sizeN; ++j) {
            inFile >> matrixA[i][j];
        }
    }

    inFile.close();

    vector<vector<int>> resultMatrix(sizeN, vector<int>(sizeN, 0));

    // Matrix multiplication using the chunk technique
    auto startChunk = chrono::high_resolution_clock::now();
    vector<thread> chunkThreads;
    try {
        for (int i = 0; i < threadsK; ++i) {
            int start = i * (sizeN / threadsK);
            int end = (i + 1) * (sizeN / threadsK);
            chunkThreads.emplace_back(MultiplicationChunk, ref(matrixA), ref(resultMatrix), start, end);
            if (i < boundedThreadsBT) {
                SetThreadAffinity(chunkThreads.back(), i % coresC);
            }
        }

        for (auto& th : chunkThreads) {
            th.join();
        }
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
    auto endChunk = chrono::high_resolution_clock::now();
    auto durationChunk = chrono::duration_cast<chrono::microseconds>(endChunk - startChunk);

    // Matrix multiplication using the mixed technique
    auto startMixed = chrono::high_resolution_clock::now();
    vector<thread> mixedThreads;
    try {
        for (int i = 0; i < threadsK; ++i) {
            mixedThreads.emplace_back(MultiplicationMixed, ref(matrixA), ref(resultMatrix), i, threadsK);
            if (i < boundedThreadsBT) {
                SetThreadAffinity(mixedThreads.back(), i % coresC);
            }
        }

        for (auto& th : mixedThreads) {
            th.join();
        }
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
    auto endMixed = chrono::high_resolution_clock::now();
    auto durationMixed = chrono::duration_cast<chrono::microseconds>(endMixed - startMixed);

    // Write the resulting matrix and time taken to the output file
    string outputFile = "out.txt";
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }
    outFile << "Resultant Square Matrix:\n";
    for (int i = 0; i < sizeN; ++i) {
        for (int j = 0; j < sizeN; ++j) {
            outFile << resultMatrix[i][j] << " ";
        }
        outFile << "\n";
    }

    outFile << "Time Taken by Chunk Method: " << durationChunk.count() / 1000000.0 << " seconds\n";
    outFile << "Time Taken by Mixed Method: " << durationMixed.count() / 1000000.0 << " seconds\n";

    outFile.close();

    return 0;
}

