/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: someone
 *
 * Created on June 29, 2016, 1:05 PM
 */

#include <cstdlib>
#include <iostream>

using namespace std;

template <typename T>
struct list_node {
    T data;
    list_node<T>* next;

    list_node(T x) : data(x), next(NULL) {
    }

    list_node<T>* create_and_push_back(list_node<T>* cn, T x) {
        cn->next = new list_node<T>(x);
        return cn->next;
    }
};

bool check_list(list_node<int> * pntr) {
    list_node<int> * node1;
    list_node<int> * node2;

    for (node1 = pntr; node1 != NULL; node1 = node1->next) {

        if (node1 == node1->next)
            return true;
        for (node2 = pntr; node1 != node2; node2 = node2->next) {
            cout << node1->next->data << "\t" << node2->data << endl;
            if (node2 == node1->next) {
                return true;
            }
        }
    }


    return false;
}

bool check_list2(list_node<int>* root) {
    list_node<int>* first = root;
    list_node<int>* second = root;
    for (;;) {
        cout << first->data << "\t" << second->data << endl;
        if (first->next == NULL) return false;
        first = first->next;
        if (first == second) return true;

        if (first->next == NULL) return false;
        first = first->next;
        if (first == second) return true;

        second = second->next;
    }
}

bool check_list3(list_node<int>* root) {
    list_node<int>* first = root;
    list_node<int>* second = root;
    for (;;) {
        cout << first->data << "\t" << second->data << endl;
        if (first->next == NULL || first->next->next == NULL) return false;
        if (first->next == second || first->next->next == second ) return true;
        first = first->next->next;
        second = second->next;
    }
}

int main(int argc, char** argv) {

    list_node<int> head(0);
    list_node<int>* it = &head;
    for (int i = 1; i < 103; i++) {
        it = it->create_and_push_back(it, i);
    }

    it->next = head.next->next->next->next;
    cout << it->data << "\t" << it->next->data << endl;

    it = &head;

    //cout << check_list(it) << endl;

    //cout << check_list3(it) << endl;

    list_node<int>* t = &head;
    list_node<int>* r = head.next;

    int cntr = 0;

    while (t!=r && r->next != NULL) {
        cout << cntr << "\t" << r->data << "\t" << t->data << "\n";
        t = t->next;
        r = r->next->next;
        ++cntr;
    }

    cout << t->data << "\t"  << r->data << "\t" << r->next->data << "\t" << r->next->next->data << "\t" << endl;
    r = &head;
    int x = 0;
    while (r != t) {
        r=r->next;
        x++;
    }

    cout << cntr << " " << x << endl;

    return 0;
}

