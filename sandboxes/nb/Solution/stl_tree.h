#ifndef STL_TREE_H
#define STL_TREE_H

level_order(TreeNode* root)
{
   queue<TreeNode*> q;
   TreeNode* cn = root;
 
    while (cn) {
        cout << cn->data;
        if (cn->left) q.Add(cn->left);
        if (cn->reght) q.Add(cn->reght);
        cn = q.Delete(); //q.Delete returns a node pointer
    }
}


#endif /* STL_TREE_H */

