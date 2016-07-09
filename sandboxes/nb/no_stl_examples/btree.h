/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   btree.h
 * Author: someone
 *
 * Created on July 5, 2016, 7:43 PM
 */

#ifndef BTREE_H
#define BTREE_H

#include <limits.h>

class btn {
public:

    btn(int data) : d(data), l(NULL), r(NULL) { }
    int d;
    btn* l;
    btn* r;

    static void insert(btn** nd, int data);
    static void insert(btn*& nd, int data);
    static void invert(btn * nd);
    static void destroy(btn *&nd);

    static int height(btn *nd);

    static void treat(btn *nd);
    static void in_order(btn *nd);
    static void pre_order(btn *nd);
    static void post_order(btn *nd);

    static bool search(btn* nd, int data);

    static bool is_bst(btn* node, int min, int max);
    static int getmax(btn *nd);
    static int secondmax(btn *nd);

    static void print(btn* node, string indent, bool is_tail);
    static void print_by_level(btn* nd, int level);
private:

};

void btn::insert(btn** nd, int data) {
    if (!nd)
        return;

    if (*nd == NULL)
        *nd = new btn(data);
    else if (data < (*nd)->d)
        insert(&((*nd)->l), data);
    else
        insert(&((*nd)->r), data);
}

void btn::insert(btn*& nd, int data) {
    if (nd == nullptr) {
        nd = new btn(data);
    } else if (data < nd->d) {
        insert(nd->l, data);
    } else {
        insert(nd->r, data);
    }
}

void btn::invert(btn * nd) {

    if (nd == NULL)
        return;

    invert(nd->l);
    invert(nd->r);

    btn * tmp = nd->l;
    nd->l = nd->r;
    nd->r = tmp;

}

void btn::destroy(btn *&nd) {
    if (!nd)
        return;

    destroy(nd->l);
    destroy(nd->r);

    delete nd;
    nd = NULL;
}

void btn::treat(btn *nd) {
    cout << nd->d << "  ";
}

int btn::height(btn* nd) {
    if (nd == NULL)
        return 0;
    else {
        /* compute the height of each subtree */
        int lh = height(nd->l);
        int rh = height(nd->r);

        /* use the larger one */
        if (lh > rh)
            return (lh + 1);
        else return (rh + 1);
    }
}

void btn::in_order(btn *nd) {
    if (!nd)
        return;
    in_order(nd->l);
    treat(nd);
    in_order(nd->r);
}

void btn::pre_order(btn *nd) {
    if (!nd)
        return;
    treat(nd);
    pre_order(nd->l);
    pre_order(nd->r);
}

void btn::post_order(btn *nd) {
    if (!nd)
        return;
    post_order(nd->l);
    post_order(nd->r);
    treat(nd);
}

bool btn::is_bst(btn* node, int min, int max) {
    if (node == NULL)
        return true;

    if (node->d < min || node->d > max)
        return false;

    return is_bst(node->l, min, node->d) && is_bst(node->r, node->d, max);
}

bool btn::search(btn* nd, int data) {
    if (nd == NULL) {
        return false;
    } else if (data == nd->d) {
        return true;
    } else if (data < nd->d) {
        return search(nd->l, data);
    } else {
        return search(nd->r, data);
    }
}

int btn::getmax(btn *nd) {
    if (nd->r == NULL) {
        return nd->d;
    }
    return getmax(nd->r);
}

int btn::secondmax(btn *nd) {
    if (nd == NULL) {
        return -1;
    }

    if (nd->r == NULL && nd->l != NULL) {
        return getmax(nd->l);
    }

    if (nd->r != NULL && nd->r->r == NULL && nd->r->l == NULL) {
        return nd->d;
    }


    return secondmax(nd->r);
}

/* Print nodes at a given level */
void btn::print_by_level(btn* nd, int level=0) {
    if (nd == NULL)
        return;
    if (level == 0) {
        int h = height(nd);
        for (int i = 1; i <= h; i++)
            print_by_level(nd, i);
    }
    if (level == 1)
        printf("%d ", nd->d);
    else if (level > 1) {
        print_by_level(nd->l, level - 1);
        print_by_level(nd->r, level - 1);
    }
}


void btn::print(btn* node, string indent, bool is_tail) {
    cout << indent << (is_tail ? "└── " : "├── ");
    if (node == NULL) {
        cout << "*" << endl;
        return;
    }
    cout << node->d << endl;
    if (node->l != NULL || node->r != NULL) {
        indent.append(is_tail ? "    " : "│   ");
        print(node->r, indent, false);
        print(node->l, indent, true);
    }
}


#endif /* BTREE_H */

