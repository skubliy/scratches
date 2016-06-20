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
    cout << endl << __func__ << endl;
    int a[] = {8, 9, 1, 3, 4, 9, 4, 6, 7, 4, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);
    vector<int> v(a, a + sz);

    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ",";
    }

    cout << endl;

    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << ",";
    }

    cout << endl;
}

void list_iterators_example() {
    cout << endl << __func__ << endl;
    int a[] = {8, 9, 1, 3, 4, 9, 4, 6, 7, 4, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);

    list<int> lst;
    //init list
    for (int i = 0; i < sz; i++) {
        lst.push_back(a[i]);
    }

    for (std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << ",";
    }
    cout << endl;
    for (std::list<int>::const_iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << ",";
    }
    cout << endl;

}

void map_iteratos() {
    cout << endl << __func__ << endl;
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

int revert_string_by_revert_iterator() {
    cout << endl << __func__ << endl;

    string str("test string");
    copy(str.rbegin(), str.rend(), std::ostream_iterator<char> (cout));
    cout << endl;
}

void back_insert_iterator() {
    cout << endl << __func__ << endl;

    std::vector<int> foo, bar;
    for (int i = 1; i <= 5; i++) {
        foo.push_back(i);
        bar.push_back(i * 10);
    }

    std::back_insert_iterator< std::vector<int> > back_it(foo);

    std::copy(bar.begin(), bar.end(), back_it);

    std::cout << "foo:";
    for (std::vector<int>::iterator it = foo.begin(); it != foo.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';
}

void insert_iterator() {
    cout << endl << __func__ << endl;
    std::list<int> foo, bar;
    for (int i = 1; i <= 5; i++) {
        foo.push_back(i);
        bar.push_back(i * 10);
    }

    std::list<int>::iterator it = foo.begin();
    advance(it, 3);

    std::insert_iterator< std::list<int> > insert_it(foo, it);

    std::copy(bar.begin(), bar.end(), insert_it);

    std::cout << "foo:";
    for (std::list<int>::iterator it = foo.begin(); it != foo.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

}

#endif /* ITERATORS_H */

