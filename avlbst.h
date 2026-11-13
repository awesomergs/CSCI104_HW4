#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* x);
    void rotateRight(AVLNode<Key, Value>* x);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    } 

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* n = NULL;

    while (true) {
        if (new_item.first < curr->getKey()) {
            if (curr->getLeft() == nullptr) {
                n = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
                curr->setLeft(n);
                break;
            }
            curr = curr->getLeft();
        } else if (curr->getKey() < new_item.first) {
            if (curr->getRight() == nullptr) {
                n = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
                curr->setRight(n);
                break;
            }
            curr = curr->getRight();
        } else {
            curr->setValue(new_item.second);
            return;
        }
    }

    if (curr->getLeft() == n) curr->updateBalance(-1);
    else                      curr->updateBalance(1);

    if (curr->getBalance() == 0) return;  
    insertFix(curr, n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if (p == nullptr) return;
    AVLNode<Key, Value>* g = p->getParent();
    if (g == nullptr) return;

    if (p == g->getLeft()) {
        g->updateBalance(-1);
        if (g->getBalance() == 0) {
            return;
        } else if (g->getBalance() == -1) {
            insertFix(g, p);
        } else { //-2 v
            if (n == p->getLeft()) {       
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            } else {                          
                rotateLeft(p);
                rotateRight(g);
                int8_t nb = n->getBalance();
                if (nb == -1)      { p->setBalance(0);  g->setBalance(1); }
                else if (nb == 0)  { p->setBalance(0);  g->setBalance(0); }
                else               { p->setBalance(-1); g->setBalance(0); }
                n->setBalance(0);
            }
        }
    } else {
        g->updateBalance(1);
        if (g->getBalance() == 0) {
            return;
        } else if (g->getBalance() == 1) {
            insertFix(g, p);
        } else { //+2 v
            if (n == p->getRight()) {           
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            } else {                        
                rotateRight(p);
                rotateLeft(g);
                int8_t nb = n->getBalance();
                if (nb == 1)       { p->setBalance(0);  g->setBalance(-1); }
                else if (nb == 0)  { p->setBalance(0);  g->setBalance(0); }
                else               { p->setBalance(1);  g->setBalance(0); }
                n->setBalance(0);
            }
        }
    }
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == nullptr) return;

    if (node->getLeft() != nullptr && node->getRight() != nullptr) {
        AVLNode<Key, Value>* pred =
            static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
        nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();

    int8_t diff = 0;
    if (parent != nullptr) {
        diff = (parent->getLeft() == node) ? 1 : -1;
    }

    if (child != nullptr) child->setParent(parent);

    if (parent == nullptr) {
        this->root_ = child;
    } else if (parent->getLeft() == node) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete node;

    removeFix(parent, diff);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    if (n == nullptr) return;

    AVLNode<Key, Value>* p = n->getParent();
    int8_t ndiff = 0;
    if (p != nullptr) ndiff = (n == p->getLeft()) ? 1 : -1;

    int8_t b = n->getBalance() + diff;

    if (diff == -1) {
        if (b == -2) {
            AVLNode<Key, Value>* c = n->getLeft();
            int8_t cb = c->getBalance();
            if (cb == -1) {                   
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            } else if (cb == 0) {              
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            } else {                          
                AVLNode<Key, Value>* g = c->getRight();
                int8_t gb = g->getBalance();
                rotateLeft(c);
                rotateRight(n);
                if (gb == 1)       { n->setBalance(0); c->setBalance(-1); }
                else if (gb == 0)  { n->setBalance(0); c->setBalance(0); }
                else               { n->setBalance(1); c->setBalance(0); }
                g->setBalance(0);
                removeFix(p, ndiff);
            }
        } else if (b == -1) {
            n->setBalance(-1);
            return;
        } else { 
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    } else { 
        if (b == 2) {
            AVLNode<Key, Value>* c = n->getRight();
            int8_t cb = c->getBalance();
            if (cb == 1) {                   
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            } else if (cb == 0) {    
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
                return;
            } else {                          
                AVLNode<Key, Value>* g = c->getLeft();
                int8_t gb = g->getBalance();
                rotateRight(c);
                rotateLeft(n);
                if (gb == -1)      { n->setBalance(0);  c->setBalance(1); }
                else if (gb == 0)  { n->setBalance(0);  c->setBalance(0); }
                else               { n->setBalance(-1); c->setBalance(0); }
                g->setBalance(0);
                removeFix(p, ndiff);
            }
        } else if (b == 1) {
            n->setBalance(1);
            return;
        } else { 
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{ //x moves down&left, right child moves up
    AVLNode<Key, Value>* y = x->getRight();
    AVLNode<Key, Value>* xp = x->getParent();

    x->setRight(y->getLeft());
    if (y->getLeft() != nullptr) y->getLeft()->setParent(x);

    y->setLeft(x);
    x->setParent(y);
    y->setParent(xp);

    if (xp == nullptr)              this->root_ = y;
    else if (xp->getLeft() == x) xp->setLeft(y);
    else                         xp->setRight(y);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x)
{ //x moves down&right, left child moves up
    AVLNode<Key, Value>* y = x->getLeft();
    AVLNode<Key, Value>* xp = x->getParent();

    x->setLeft(y->getRight());
    if (y->getRight() != nullptr) y->getRight()->setParent(x);

    y->setRight(x);
    x->setParent(y);
    y->setParent(xp);

    if (xp == nullptr)              this->root_ = y;
    else if (xp->getLeft() == x) xp->setLeft(y);
    else                         xp->setRight(y);
}


#endif