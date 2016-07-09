

#ifndef LIST_NODE_H
#define LIST_NODE_H

typedef struct inode {
    int data;
    inode* next;
}
int_node_t;

template <typename T>
class List {

    template<typename TT>
    struct node {
        TT data;
        node<TT>* next;
    };

    node<T>* head;

public:

    List() {
        head = NULL;
    }

    ~List() {
        while (head != NULL) {
            node<T> * n = head->next;
            delete head;
            head = n;
        }
    }

    void add(T& value) {
        node<T> * n = new node<T>();
        n->data = value;
        n->next = head;
        head = n;
        std::cout << head << ":\t" << n->next << "\t" << n->data << std::endl;
    }

    void print() {
        node<T>* n = head;
        while (n != NULL) {
            std::cout << n->data << ", ";
            n = n->next;
        }
        std::cout << std::endl;
    }
};

void test_list() {
    List<int> lst;
    for (int i = 0; i < 8; ++i) {
        lst.add(i);
    }
    lst.print();

    return;
}

#endif /* LIST_NODE_H */

