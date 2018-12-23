## Float_128

# General info
Project implements floating point number of size 128 bits.

![](images/float.png?raw=true )
Number consists of:
- 1-bit sign
- 13-bit exponent 
- 114-bit mantissa 


Formula to compute decimal number:
![](images/formula.png?raw=true )

Where:
 - s - sign
 - E - exponent
 - M - mantissa
 - bias = 4095


Number is stored in two-element uint64_t array. 


# Running sample project
Linux:
```make
cd bin
./float_128
```

To clear the project, type:
```
make clear
```

# Running tests
Running tests requires installation of:
- gtest

Installation on Ubuntu:
```
apt-get install libgtest-dev
apt-get install cmake
cd ~
g++ -I /usr/include/gtest -I /usr/src/gtest/ -c /usr/src/gtest/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o
mv libgtest.a /usr/lib/

```
Linux:
```
cd tests
g++ main_tests.cpp ../src/float_128.cpp -o test -lgtest 
./test
```

# Application
Big-size floating point numbers can be used for storing numbers with high accuracy. Also they can be used for performing arithmetical operations without significant data loss. 
