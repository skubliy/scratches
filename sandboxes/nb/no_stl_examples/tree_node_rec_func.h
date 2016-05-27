#ifndef TREE_NODE_REC_FUNC_H
#define TREE_NODE_REC_FUNC_H

#include <queue>

using namespace std;

typedef struct treenode_t {
    int data;
    treenode_t *parent;
    treenode_t *left;
    treenode_t *right;
} TreeNode;

void treat(TreeNode* cn) {
    cout << cn->data << " ";
};

void in_order(TreeNode* cn) {
    if (cn) {
        in_order(cn->left);
        treat(cn);
        in_order(cn->right);
    }
}

void pre_order(TreeNode* cn) {
    if (cn) {
        treat(cn);
        pre_order(cn->left);
        pre_order(cn->right);
    }
}

void post_order(TreeNode* cn) {
    if (cn) {
        post_order(cn->left);
        post_order(cn->right);
        treat(cn);
    }
}

level_order(TreeNode* root) {
    queue<TreeNode*> q;
    TreeNode* cn = root;
    while (cn) {
        treat(cn);
        if (cn->left) q.push(cn->left);
        if (cn->right) q.push(cn->right);
        cn = q.pop();
    }
}

TreeNode* find_next_largest_element(TreeNode *node) { };

#endif /* TREE_NODE_REC_FUNC_H */

