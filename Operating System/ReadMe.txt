Parallel Matrix Multiplication by Thread Affinity:-
Compilation
Compile the program using a C++ compiler. For example, using g++:
g++ Assgn2_Src-CS22BTECH11035.cpp 
./a.out

Input File Format (inp.txt)
The input file (inp.txt) should have the following format:
N K C BT
A[0][0] A[0][1] ... A[0][N-1]
A[1][0] A[1][1] ... A[1][N-1]
...
A[N-1][0] A[N-1][1] ... A[N-1][N-1]
Here, N is the number of rows of matrix A, K is the number of threads, C is the number of logical cores and BT is the number of bounded threads.

Output File Format (out.txt)
The program will generate an output file (out.txt) containing the following:
Resulting Square Matrix:
C[0][0] C[0][1] ... C[0][N-1]
C[1][0] C[1][1] ... C[1][N-1]
...
C[N-1][0] C[N-1][1] ... C[N-1][N-1]
Time taken to compute the square matrix: X seconds both by chunk and mixed type. 

Example
An example input file (inp.txt) and expected output file (out.txt) are provided for testing purposes.
