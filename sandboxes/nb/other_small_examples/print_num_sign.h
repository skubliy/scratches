
#ifndef PRINT_NUM_SIGN_H
#define PRINT_NUM_SIGN_H

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;

void print_num_sings(int n) {
    cout << endl << __func__ << endl;
    char arr[n];
    for (int x = 0; x < n - 1; x++) {
        arr[x] = ' ';
    }
    arr[n] = '\0';
    for (int x = 1; x <= n; x++) {
        arr[n - x] = '#';
        cout << arr << endl;
    }
}

void print_num_sings_by_formating_cout(int n) {
    cout << endl << __func__ << endl;
    char arr[n];

    for (int x = 0; x < n; x++) {
        arr[x] = '#';
        arr[x + 1] = '\0';
        cout << setw(n) << &arr[0] << endl;
    }
}


#endif /* PRINT_NUM_SIGN_H */

