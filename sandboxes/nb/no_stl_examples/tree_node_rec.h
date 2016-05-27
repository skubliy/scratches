#ifndef TREE_NODE_REC_H
#define TREE_NODE_REC_H

typedef struct tn_t {
    int d;
    tn_t *p;
    tn_t *l;
    tn_t *r;
}
int_nd_t;

template< typename T > class btree;

template<typename T>
class tn_tt {
    friend class btree< T >;
public:

    tn_tt() : l(0), r(0) { }

    tn_tt(const T &data) : d(data), l(0), r(0) { }

    T get_data() const {
        return d;
    }
private:
    T d;
    tn_tt *l;
    tn_tt *r;
};
typedef tn_tt<int> int_nd_tt;

template< typename Tp >
Tp max(const Tp &left, const Tp &right) {
    return left > right ? left : right;
}

//traversals

template<typename T>
class tree {
    template< typename Tp >
    friend Tp max(const Tp &, const Tp &);

public:

    tree() : rt(0) { }

    ~tree() {
        del(rt);
    }

    void pre_order() const {
        pre_order(rt);
    }

    void in_order() const {
        in_order(rt);
    }

    void post_order() const {
        post_order(rt);
    }

    int depth() const {
        return depth(rt);
    }

    void insert(const T &data) {
        insert(&rt, data);
    }

    void remove(const T &data) {
        remove(&rt, data);
    }

    void print() const {
        print(rt, 0);
    }

private:
    tn_tt<T> *rt;

    void pre_order(tn_tt< T > *nd) const {
        if (nd != 0) {
            treat();
            pre_order(nd->l);
            pre_order(nd->r);
        }
    }

    void in_order(tn_tt< T > *nd) const {
        if (nd != 0) {
            in_order(nd->l);
            treat();
            in_order(nd->r);
        }
    }

    void post_order(tn_tt< T > *nd) const {
        if (nd != 0) {
            post_order(nd->l);
            post_order(nd->r);
            treat();

        }
    }

    void treat(tn_tt< T > *nd) {
        cout << nd->d << "  ";
    }

    int depth(tn_tt< T > *nd) const {
        if (nd->l == 0 && nd->r == 0)
            return 1;
        else {
            if (nd->l == 0)
                return 1 + depth(nd->r);
            else {
                if (nd->r == 0)
                    return 1 + depth(nd->l);
                else
                    return 1 + max(depth(nd->l), depth(nd->r));
            }
        }
    }

    void del(tn_tt< T > *nd) {
        if (nd != 0) {
            del(nd->l);
            del(nd->r);
            delete nd;
        }
    }

    void insert(tn_tt< T > **nd, const T &data) {
        if (*nd == NULL)
            *nd = new tn_tt< T > (data);
        else if ((*nd)->d > data)
            insert(&((*nd)->l), data);
        else
            insert(&((*nd)->r), data);

    }

    void remove(tn_tt< T > **nd, const T &data) {
        if ((*nd)->d == data) {
            tn_tt< T > *del_nd = *nd;

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
                    tn_tt< T > *p = *nd;
                    tn_tt< T > *i = (*nd)->l;

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
            remove(&((*nd)->l), data);
        else
            remove(&((*nd)->r), data);
    }

    void print(tn_tt< T > *nd, int spaces) const {
        while (nd != 0) {
            print(nd->r, spaces + 5);

            for (int i = 1; i < spaces; ++i)
                cout << ' ';
            cout << nd->d << std::endl;

            nd = nd->l;
            spaces += 5;
        }
    }
};


#endif /* TREE_NODE_REC_H */

