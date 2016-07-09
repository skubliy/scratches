/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: someone
 *
 * Created on June 23, 2016, 7:35 PM
 */

#include <cstdlib>
#include <iostream>

#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
    int arr[] = {1, 2, 3};
    std::for_each(&arr[0], &arr[0] + sizeof (arr) / sizeof (arr[0]), [](int n) {
        std::cout << n << std::endl;
    });
    return 0;
}

