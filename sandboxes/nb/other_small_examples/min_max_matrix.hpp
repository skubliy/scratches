/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   min_max_matrix.hpp
 * Author: someone
 *
 * Created on June 20, 2016, 6:43 PM
 */

#ifndef MIN_MAX_MATRIX_HPP
#define MIN_MAX_MATRIX_HPP

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

#endif /* MIN_MAX_MATRIX_HPP */

