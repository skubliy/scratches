
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <list>

#include "print_num_sign.h"
#include "iterators.h"
#include "min_max_matrix.hpp"
#include "reversing.hpp"

using namespace std;

int main(int argc, char** argv) {
    const char* acstr = "0123456789";
    char astr[strlen(acstr)];
    strcpy(astr, acstr);
    cout << astr << endl;

    reverse_iter(astr);
    cout << astr << endl;

    reverse_recursive(astr);
    cout << astr << endl;

    vector_iterators_example();
    list_iterators_example();
    //test_x();
    back_insert_iterator_example();
    matrix_min_max();
    revert_string_by_revert_iterator();
    print_num_sings(6);
    print_num_sings_by_formating_cout(6);

    return 0;
}



