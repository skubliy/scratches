#ifndef BTREE_H
#define BTREE_H

#include <sstream>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>

using namespace std;

template< typename T > class btree;

template<typename T>
class btnode {
    friend class btree<T>;
public:

    btnode() : l(0), r(0) { }

    btnode(const T &data) : d(data), l(0), r(0) { }

    T data() const {
        return d;
    }
private:
    T d;
    btnode *l;
    btnode *r;
};

//btree

template<typename T>
class btree {
    btnode<T> *rt;
public:

    btree() : rt(NULL) { }

    ~btree() {
        del(rt);
    }

    void in_order() const {
        in_order(rt);
    }

    void in_order(btnode< T > *nd) const {
        if (!nd)
            return;
        in_order(nd->l);
        treat(nd);
        in_order(nd->r);
    }

    void pre_order() const {
        pre_order(rt);
    }

    void pre_order(btnode< T > *nd) const {
        if (!nd)
            return;
        treat(nd);
        pre_order(nd->l);
        pre_order(nd->r);
    }

    void post_order() const {
        post_order(rt);
    }

    void post_order(btnode< T > *nd) const {
        if (!nd)
            return;
        post_order(nd->l);
        post_order(nd->r);
        treat(nd);
    }

    int depth() const {
        depth(rt);
    }

    int depth(btnode< T > *nd) const {
        if (!nd)
            return;
        if (nd->l == NULL && nd->r == NULL)
            return 1;
        else
            if (nd->l == NULL)
            return 1 + depth(nd->r);
        else if (nd->r == NULL)
            return 1 + depth(nd->l);
        else
            return 1 + (nd->l > nd->r) ? depth(nd->l) : depth(nd->r);
    }

    void insert(const T& data) {
        insert(data, rt);
    }

    /*
    void insert(const T& data, btnode< T > **nd) {
        if (!nd)
            return;
        if (*nd == NULL)
     *nd = new btnode< T > (data);
        else if ((*nd)->d > data)
            insert(data, &((*nd)->l));
        else
            insert(data, &((*nd)->r));
    }
     */
    void insert(const T& data, btnode< T >* & nd) {
        if (!nd) {
            nd = new btnode< T > (data);
            return;
        }
        insert(data, nd->d > data ? nd->l : nd->r);
    }

    void remove(const T &data) {
        remove(data, rt);
    }

    void remove(const T &data, btnode< T > **nd) {
        if (!nd || !(*nd))
            return;
        if ((*nd)->d == data) {
            btnode< T > *del_nd = *nd;

            if ((*nd)->l == NULL && (*nd)->r == NULL) {
                *nd = NULL;
                delete del_nd;
            } else if ((*nd)->l == NULL) {
                *nd = (*nd)->r;
                delete del_nd;
            } else {
                if ((*nd)->r == NULL) {
                    *nd = (*nd)->l;
                    delete del_nd;
                } else {
                    btnode< T > *p = *nd;
                    btnode< T > *i = (*nd)->l;

                    while (i->r != NULL) {
                        p = i;
                        i = i->r;
                    }

                    *nd = i;
                    p->r = i->l;
                    i->r = del_nd->r;
                    i->l = p;
                    delete del_nd;
                }
            }
        } else if ((*nd)->d > data)
            remove(data, &((*nd)->l));
        else
            remove(data, &((*nd)->r));
    }

    // void print() {
    //     print(16, rt);
    // }

    deque<stringstream*> sss;

    /*
    void print() {
        stringstream* ln = new stringstream();
        print(ln, 8, rt);
        cout << ln->str() << endl;
        while (sss.size()) {
            const stringstream* ss = sss.back();
            sss.pop_back();
            cout << ss->str() << endl;
            delete ss;
        }

    }

    void print(stringstream* ln, int spaces, btnode< T > *nd) {
        if (!nd)
            return;

        stringstream* ln2 = new stringstream();

        print(ln2, spaces+1, nd->l);
        cout << "--" << ln2->str()  << " | ";

        print(ln2, spaces+3, nd->r);
        cout << ln2 -> str() << endl;
        sss.push_back(ln2);
        for (int i = 1; i < spaces; ++i)
     *ln << " ";
     *ln << nd->d;
    }*/

    void postorder_print() {
        postorder_print(rt, 16);
    }

    void postorder_print(btnode< T >* p, int indent) {
        if (p != NULL) {
            if (p->r) {
                postorder_print(p->r, indent + 4);
            }
            if (indent) {
                cout << setw(indent) << ' ';
            }
            if (p->r) cout << " /\n" << setw(indent) << ' ';
            cout << p->d << "\n ";
            if (p->l) {
                cout << setw(indent) << ' ' << " \\\n";
                postorder_print(p->l, indent + 4);
            }
        }
    }

    void print2() {
        string s("");
        print2(rt, s, true);
    }

    void print2(btnode< T >* node, string indent, bool is_tail) {
        cout << indent << (is_tail ? "└── " : "├── ");
        if (node == NULL) {
            cout << "*" << endl;
            return;
        }
        cout << node->d << endl;
        if (node->l != NULL || node->r != NULL) {
            indent.append(is_tail ? "    " : "│   ");
            print2(node->r, indent, false);
            print2(node->l, indent, true);
        }
    }

    void del(btnode< T > *nd) {
        if (!nd)
            return;
        del(nd->l);
        del(nd->r);
        delete nd;
    }

    void treat(btnode< T > *nd) const {
        cout << nd->d << "  ";
    }

};


#endif /* BTREE_H */

