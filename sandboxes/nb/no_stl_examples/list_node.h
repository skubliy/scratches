

#ifndef LIST_NODE_H
#define LIST_NODE_H

typedef struct node_t {
    int data;
    node_t* next;
}
int_node_t;

template <typename T>
struct node_tt {
    T data;
    node_tt<T>* next;
};

#endif /* LIST_NODE_H */

