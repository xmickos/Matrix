# (1) HWMX: Calculating determinant 
Input: size elem11 elem12 elem13 ... (input elements row-by-row) <br> 
Output: determinant
### Example: 
Input: 2 1 0 0 1 <br>
Output: 1
## How to build the project
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cd build
make
```
## How to run tests
```
cd build
ctest
```
## General algorithm:
Solution based on Gaussian elimination for all types except int and on Bird's division-free algorithm for integers. <br>
Original paper: __(1,2) Bird, R. S. (2011). A simple division-free algorithm for computing determinants. Inf. Process. Lett., 111(21), 1072-1074. doi: 10.1016/j.ipl.2011.08.006 –__

# (2) HWMX2: MatrixChain
Input: size <newline> col_1 row_1 row_2 row_3 row_4 ... row_n <br> 
Output: (optimal multiplitcation order indexes) <br> (multiplier how many times is the optimal order better than the order 0-1-2-...)
### Example: 
Input: <br> 5 <br>
10 5 10 5 10 5 10 <br>
Output: <br> 130 <br> 1.5
## How to build the project
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cd build
make
```
## How to run the project
```
cd build
./matrixchain
```
## How to run tests
```
cd build
ctest
```


