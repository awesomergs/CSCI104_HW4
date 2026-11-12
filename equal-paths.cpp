#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int pathLen(Node * root){

    if (root==nullptr){
        return 0;
    }

    bool r = false, l = false;
    if (root->right != nullptr ) {
        r = true;
    }
    if (root->left != nullptr) {
        l = true;
    }

    if (r && l){
        int r_val = pathLen(root->right);
        int l_val = pathLen(root->left);
        if (r_val != l_val){
            return -1;
        } else if (r_val == -1){
            return r_val;
        } else {
         return r_val + 1;
        }
    } else if (r) {
        int r_val = pathLen(root->right);
        if (r_val == -1){
            return -1;
        } else {
            return r_val + 1;
        }
    } else if (l){
        int l_val = pathLen(root->left);
        if (l_val == -1){
            return -1;
        } else {
            return l_val + 1;
        }
    } else{
        return 1;
    }

}


bool equalPaths(Node * root)
{
    if (root==nullptr){
        return true;
    }

    int x = pathLen(root);
    if (x == -1){
        return false;
    } else { return true; }

}
