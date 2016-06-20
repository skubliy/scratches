
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <list>

#include "print_num_sign.h"

#include "iterators.h"

using namespace std;


void matrix_min_max() {
    cout << endl << __func__ << endl;
    int imin = 0, jmin = 0, imax = 0, jmax = 0, result;
    int n = 4;
    int m = 3;
    int matr[n][m];
    //init matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            matr[i][j] = (10 - i + j + 1)*(j + 1);
        }
    }
    //lookup
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            if (matr[i][j] < matr[imin][jmin]) {
                imin = i;
                jmin = j;
            } else
                if (matr[i][j] > matr[imax][jmax]) {
                imax = i;
                jmax = j;
            }
    }
    //output
    for (int i = 0; i < n; ++i, std::cout << std::endl) {
        for (int j = 0; j < m; ++j)
            std::cout << matr[i][j] << '\t';
    }

    std::cout << "max - min = matr[" << imax << "][" << jmax << "] - matr[" << imin << "][" << jmin << "] = " << (matr[imax][jmax] - matr[imin][jmin]) << std::endl;
}

int main(int argc, char** argv) {
    vector_iterators_example();
    list_iterators_example();
    //test_x();
    back_insert_iterator();
    matrix_min_max();
    revert_string_by_revert_iterator();
    print_num_sings(6);
    print_num_sings_by_formating_cout(6);

    return 0;
}



