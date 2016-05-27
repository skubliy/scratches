
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <list>

#include "print_num_sign.h"

#include "iterators.h"

using namespace std;
/*
class A
{
public:
    A(int n = 0)
        : m_n(n)
    {
        std::cout << 'd';
    }

    A(const A& a)
        : m_n(a.m_n)
    {
        std::cout << 'c';
    }

private:
    int m_n;
};

void f(const A &a1, const A &a2 = A())
{
}

int test_x()
{
    A a(2), b;
    const A c(a), &d = c, e = b;
    b = d;
    A *p = new A(c), *q = &a;
    static_cast<void>(q);
    delete p;
    f(3);
    std::cout << std::endl;

    return 0;
}
 */

/*
class A1
{
public:
    A1(int n = 0) : m_n(n) { }

public:
    virtual int f1() const { return m_n; }
    virtual ~A1() { }

protected:
    int m_n;
};

class B
    : public A1
{
public:
    B(int n = 0) : A1(n) { }

public:
    virtual int f1() const { return m_n + 1; }
};

int main1()
{
    const A1 a(1);
    const B b(3);
    const A1 *x[2] = { &a, &b };
    typedef std::vector<A1> V;
    A1 arr[] = { a, b };
    V y(arr, arr + sizeof (arr) / sizeof (*arr));
    V::const_iterator i = y.begin();

    std::cout << x[0]->f1() << x[1]->f1()
              << i->f1() << (i + 1)->f1() << std::endl;
    
    return 0;
}

 */

class A {
public:

    A(int n = 2) : m_i(n) { }

    ~A() {
        std::cout << m_i;
    }

protected:
    int m_i;
};

class B
: public A {
public:

    B(int n) : m_a1(m_i + 1), m_a2(n) { }

public:

    ~B() {
        std::cout << m_i;
        --m_i;
    }

private:
    A m_a1;
    A m_a2;
};

int main3() {
    {
        B b(5);
    }

    std::cout << std::endl;

    return 0;
}

template<typename T>
class vh {
public:

    static void print_all(vector<T>& v) {
        for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
            cout << *it << "  ";
        //cout << endl;
    }
};

void print_nge(int arr[], int n) {
    int next, i, j;
    for (i = 0; i < n; i++) {
        next = -1;
        for (j = i + 1; j < n; j++) {
            if (arr[i] < arr[j]) {
                next = arr[j];
                break;
            }
        }
        cout << arr[i] << " -- " << next << endl;
    }
}

void test_print_nge() {
    int a[] = {8, 9, 1, 3, 4, 9, 4, 6, 7, 4, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);
    vector<int> v(a, a + sz);
    vh<int>::print_all(v);
    cout << endl;
    print_nge(a, sz);
}

int main4() {
    int arr[] = {11, 13, 21, 3};
    int n = sizeof (arr) / sizeof (arr[0]);
    print_nge(arr, n);
    return 0;
}

int revert_string() {
    string str("test string");
    copy(str.rbegin(), str.rend(), std::ostream_iterator<char> (cout));
}

void back_test() {
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

void matrix_min_max() {
    int imin = 0, jmin = 0, imax = 0, jmax = 0, result;
    int n = 4;
    int m = 3;
    int matr[n][m] ;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            matr[i][j]=(10-i+j+1)*(j+1);
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
            std::cout << matr[i][j] << ' ';
    }

    std::cout << "\nmax - min = matr[" << imax << "][" << jmax << "] - matr[" << imin << "][" << jmin << "] = " << (matr[imax][jmax] - matr[imin][jmin]) << std::endl;
}



int main(int argc, char** argv) {
    vector_iterators_example();
    list_iterators_example();
    //test_x();
    matrix_min_max();
    revert_string();
    main4();
    test_print_nge();
    main3();
    print_num_sings(6);
    print_num_sings_by_formating_cout(6);

    return 0;
}



