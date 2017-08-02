//
// Created by Seth on 8/2/17.
//
#include <iostream>

#include "yanda.hpp"

//// Simplify with typedefs as needed ////
using Array4D = yanda::NDimensionalArray<int, 4>;
using Array3D = yanda::NDimensionalArray<int, 3>;
using Array2D = yanda::NDimensionalArray<int, 2>;
using Idx = Array4D::Index;

int main()
{
    //// 4D Array ////
    // Equal to int[4][3][2][1]
    Array4D::Extent size4{5, 4, 3, 2};
    Array4D array4(size4);

    // Assignment
    array4({4, 3, 2, 1}) = 18;

    // Retrieval
    int val = array4({4, 3, 2, 1});

    //// 3D array ////
    // Initialize the Extents with a list
    Array3D array3({4, 3, 2});

    // Expand the array
    array3.setExtents({4, 4, 4});

    // Fill the array
    for (Idx z = 0; z < array3.extents()[0]; z++) {
        for (Idx y = 0; y < array3.extents()[1]; y++) {
            for (Idx x = 0; x < array3.extents()[2]; x++) {
                array3({z, y, x}) = val++;
            }
        }
    }
    yanda::Print(array3);

    //// 2D array ////
    // Get 2D array by slicing the 3D array
    Array2D array2 = array3.slice(2);
    yanda::Print(array2);

    // Get raw data
    std::vector<int> data = array2.data();

    // Make new array from raw data
    Array2D array2_2({8, 2}, data.begin(), data.end());
    yanda::Print(array2_2);

    // Try to make bad array
    try {
        Array2D array2_3({5, 3}, data.begin(), data.end());
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}