# YANDA
Yet Another N-Dimensional Array

This is a header-only N-dimensional array written for C++11 using 
only STL types and templating.

## Installation
Copy `yanda.hpp` to your `include` directory. 

The example app is configured using CMake:
```
$ mkdir build/
$ cd build/
$ cmake ..
$ make
$ ./yanda-usage
```

## Usage
```
#include <yanda.hpp>

using Box = yanda::NDimensionalArray<int, 3>;

Box b(4, 3, 2);
b(1, 1, 1) = 7;

std::cout << b(1, 1, 1) << std::endl // prints '7'
```

See `examples/Usage.cpp` for more complex examples.