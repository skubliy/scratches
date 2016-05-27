#ifndef ITERATORS_H
#define ITERATORS_H
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <stack>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <map>

#include <iterator>

#include <algorithm>

#include <iomanip>
using namespace std;

typedef std::map<std::string, std::string> inmap_t;
typedef std::map<std::string, inmap_t> outmap_t;

void vector_iterators_example() {
    int a[] = {8, 9, 1, 3, 4, 9, 4, 6, 7, 4, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);
    vector<int> v(a, a + sz);

    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << std::endl;
    }

    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << endl;
    }
}

void list_iterators_example() {
    int a[] = {8, 9, 1, 3, 4, 9, 4, 6, 7, 4, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);

    list<int> lst;
    //init list
    for (int i = 0; i < sz; i++) {
        lst.push_back(a[i]);
    }


    for (std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << std::endl;
    }

    for (std::list<int>::const_iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << endl;
    }
}

void map_iteratos() {
    outmap_t mm;
    for (outmap_t::iterator i = mm.begin(); i != mm.end(); ++i) {
        inmap_t &im = i->second;
        for (inmap_t::iterator ii = im.begin(); ii != im.end(); ++ii) {
            std::cout << "map["
                    << i->first
                    << "]["
                    << ii->first
                    << "] ="
                    << ii->second
                    << '\n';
        }
    }
}


#endif /* ITERATORS_H */

