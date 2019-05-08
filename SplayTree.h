//
// Created by Yuval on 01/12/2017.
//

#ifndef COLOSSEUM_SPLAYTREE_H
#define COLOSSEUM_SPLAYTREE_H

#include <cstdio>
#include <exception>
#include "Exceptions.h"
/*
 * Classes for Exceptions:
 */
class ElementAlreadyExists : public failureException {
};

class ElementDoesntExist : public failureException {
};
/*
 * Procedure interface for procedures on the tree data.
 */
template<class T>
class Procedure {
public:
    Procedure() {};

    virtual ~Procedure() {};

    virtual void operator()(T val) = 0;
};

/*
 * Comparator interface that allows the user to decide how the splay tree should sort the elements.
 */
template<class T>
class Comparator {
public:
    virtual ~Comparator() {};

    virtual int operator()(T a, T b)=0;
};
/**
 * A template class that represents a splay tree. The class can store only pointers.
 * Every splay tree that is created by this class must be dynamically allocated.
 * The tree is defined recursively: Every splayTree has a left_son splay tree and a right_son splay tree.
 * @tparam T the type that is stored in the splay tree.
 */
template<class T>
class SplayTree {
    T data;
    bool empty;
    Comparator<T> &comparator;
    SplayTree *left_son;
    SplayTree *right_son;
    SplayTree *father;
/**
 * the rotates that are defined on the splay tree.("zig","zig-zig",zig-zag").
 * the rotates are based on the slides from the tutorial. (that is the reason for the names like A_TREE etc...)
 */
    /*
     * "zig"-left
     */
    void rotateL() {
        T x = left_son->data;
        T y = data;
        SplayTree *A_Tree = left_son->left_son;
        SplayTree *B_Tree=left_son->right_son;
        SplayTree *C_Tree = right_son;
        data = x;
        left_son->data = y;
        SplayTree *Y_New_Tree = left_son;
        left_son = A_Tree;
        right_son = Y_New_Tree;
        Y_New_Tree->right_son = C_Tree;
        Y_New_Tree->left_son=B_Tree;
        if (A_Tree) {
            A_Tree->father = this;
        }
        if(B_Tree){
            B_Tree->father=Y_New_Tree;
        }
        if (C_Tree) {
            C_Tree->father = right_son;
        }
    }
    /*
     * "zig"-right
     */
    void rotateR() {
        T x = data;
        T y = right_son->data;
        SplayTree *A_Tree = left_son;
        SplayTree *B_Tree = right_son->left_son;
        SplayTree *C_Tree = right_son->right_son;
        data = y;
        right_son->data = x;
        SplayTree *X_New_Tree = right_son;
        left_son = X_New_Tree;
        right_son = C_Tree;
        X_New_Tree->left_son = A_Tree;
        X_New_Tree->right_son=B_Tree;
        if (A_Tree) {
            A_Tree->father = left_son;
        }
        if(B_Tree){
            B_Tree->father=X_New_Tree;
        }
        if (C_Tree) {
            C_Tree->father = this;
        }
    }
    /*
     * "zig-zig"-left left.
     */
    void rotateLL() {
        T z = data;
        T x = left_son->left_son->data;
        SplayTree *A_Tree = left_son->left_son->left_son;
        SplayTree *B_Tree = left_son->left_son->right_son;
        SplayTree *C_Tree = left_son->right_son;
        SplayTree *D_Tree = right_son;
        data = x;
        left_son->left_son->data = z;
        SplayTree *Y_Tree = left_son;
        SplayTree *Z_New_Tree = left_son->left_son;
        left_son = A_Tree;
        right_son = Y_Tree;
        Y_Tree->left_son = B_Tree;
        Y_Tree->right_son = Z_New_Tree;
        Z_New_Tree->left_son = C_Tree;
        Z_New_Tree->right_son = D_Tree;
        if(D_Tree) {
            D_Tree->father = Z_New_Tree;
        }
        if(C_Tree) {
            C_Tree->father = Z_New_Tree;
        }
        Z_New_Tree->father = Y_Tree;
        if(B_Tree) {
            B_Tree->father = Y_Tree;
        }
        Y_Tree->father = this;
        if(A_Tree) {
            A_Tree->father = this;
        }
    }
    /*
     * "zig-zig"-right right.
     */
    void rotateRR() {
        T x = data;
        T z = right_son->right_son->data;
        SplayTree *A_Tree = left_son;
        SplayTree *B_Tree = right_son->left_son;
        SplayTree *C_Tree = right_son->right_son->left_son;
        SplayTree *D_Tree = right_son->right_son->right_son;
        data = z;
        right_son->right_son->data = x;
        SplayTree *Y_Tree = right_son;
        SplayTree *X_New_Tree = right_son->right_son;
        left_son = Y_Tree;
        right_son = D_Tree;
        Y_Tree->left_son = X_New_Tree;
        Y_Tree->right_son = C_Tree;
        X_New_Tree->left_son = A_Tree;
        X_New_Tree->right_son = B_Tree;
        if(A_Tree) {
            A_Tree->father = X_New_Tree;
        }
        if(B_Tree) {
            B_Tree->father = X_New_Tree;
        }
        X_New_Tree->father = Y_Tree;
        if(C_Tree) {
            C_Tree->father = Y_Tree;
        }
        Y_Tree->father = this;
        if(D_Tree) {
            D_Tree->father = this;
        }
    }
    /*
     * "zig-zig"-left right.
     */
    void rotateLR() {
        T z = data;
        T x = left_son->right_son->data;
        SplayTree *B_Tree = left_son->right_son->left_son;
        SplayTree *C_Tree = left_son->right_son->right_son;
        SplayTree *D_Tree = right_son;
        SplayTree *Y_Tree = left_son;
        data = x;
        left_son->right_son->data = z;
        SplayTree *Z_New_Tree = left_son->right_son;
        right_son = Z_New_Tree;
        Y_Tree->right_son = B_Tree;
        Z_New_Tree->left_son = C_Tree;
        Z_New_Tree->right_son = D_Tree;
        if(B_Tree) {
            B_Tree->father = Y_Tree;
        }
        if(D_Tree) {
            D_Tree->father = Z_New_Tree;
        }
        Z_New_Tree->father = this;

    }
    /*
     * "zig-zig"-right left.
     */
    void rotateRL() {
        T z = data;
        T x = right_son->left_son->data;
        SplayTree *A_Tree = left_son;
        SplayTree *B_Tree = right_son->left_son->left_son;
        SplayTree *C_Tree = right_son->left_son->right_son;
        SplayTree *Y_Tree = right_son;
        data = x;
        right_son->left_son->data = z;
        SplayTree *Z_New_Tree = right_son->left_son;
        left_son = Z_New_Tree;
        Z_New_Tree->left_son = A_Tree;
        Z_New_Tree->right_son = B_Tree;
        Y_Tree->left_son = C_Tree;
        if(A_Tree) {
            A_Tree->father = Z_New_Tree;
        }
        if(C_Tree) {
            C_Tree->father = Y_Tree;
        }
        Z_New_Tree->father = this;


    }
    /**
     * find an existing splayTree that holds a specific element using a binary search method.
     * @param element - the element to find.
     * @return - the node that holds the element.
     */
    SplayTree<T> *findBinary(const T &element) {
        int result = comparator(element, data);
        if (result == 0 || (!left_son && !right_son)) {
            return this;
        } else if (result == -1 && left_son) {
            return left_son->findBinary(element);
        } else if (result == 1 && right_son) {
            return right_son->findBinary(element);
        } else {
            return this;
        }
    }
    /**
     * recursive function that splays a node to the root of the tree.
     * @param node_to_splay - the node to splay to the root.
     */
    void splaying(SplayTree *node_to_splay) {
        if(node_to_splay==this){
            return;
        }
        if (node_to_splay->father) {
            SplayTree *grandpa = node_to_splay->father->father;
            if (grandpa && grandpa!=father) {
                if (grandpa->left_son) {
                    if (grandpa->left_son->left_son == node_to_splay) {
                        grandpa->rotateLL();
                        splaying(grandpa);
                        return;
                    } else if (grandpa->left_son->right_son == node_to_splay) {
                        grandpa->rotateLR();
                        splaying(grandpa);
                        return;
                    }
                }
                if(grandpa->right_son) {
                    if (grandpa->right_son->left_son == node_to_splay) {
                        grandpa->rotateRL();
                        splaying(grandpa);
                        return;
                    } else if (grandpa->right_son->right_son == node_to_splay) {
                        grandpa->rotateRR();
                        splaying(grandpa);
                        return;
                    }
                }
            } else {
                if (node_to_splay->father->left_son == node_to_splay) {
                    node_to_splay->father->rotateL();
                    return;
                } else if (node_to_splay->father->right_son == node_to_splay) {
                    node_to_splay->father->rotateR();
                    return;
                }
            }
        }
    }

    /**
     * splays an element to the root of the tree.
     * it finds the node to splay using the function InsertBinary, and then it splays the element using splaying.
     * @param element -
     */
    void splay(const T &element) {
        if (empty) {
            throw ElementDoesntExist();
        }
        SplayTree<T> *node_to_splay = findBinary(element);
        if (comparator(node_to_splay->data, this->data) == 0) {
            return;
        }
        splaying(node_to_splay);

    }
    /**
     * inserts an element to the tree using the normal binary insert method.
     * @param element - the element to insert.
     */
    void insertBinary(T element) {
        int result = comparator(element, data);
        if (result > 0) {
            if (!right_son) {
                right_son = new SplayTree(comparator, element);
                right_son->father = this;
                return;
            }
            right_son->insertBinary(element);
        } else if (result < 0) {
            if (!left_son) {
                left_son = new SplayTree(comparator, element);
                left_son->father = this;
                return;
            }
            left_son->insertBinary(element);
        } else {
            throw ElementAlreadyExists();
        }
    } //works
    /**
     * removes an element from the tree.
     * if the tree has a left son it splays the maximum element in it and then sets the left_sons'
     * right son to be the right son of this tree.
     * otherwise just returns the right_son.
     * pay attention that this method is private because the user should use the function removeElement.
     * @param elem_to_kick - the element to remove.
     * @return - the new root of the splay tree.
     */
    SplayTree *kick(T elem_to_kick) {
        find(elem_to_kick);
        if (comparator(elem_to_kick, this->data)) {
            throw ElementDoesntExist();
        }
        if (!left_son && !right_son) {
            empty = true;
            return this;
        }
        SplayTree *result;
        if (left_son) {
            left_son->splay(left_son->findMax());
            left_son->right_son = right_son;
            if(right_son) {
                right_son->father = left_son;
            }
            right_son = NULL;
            result = left_son;
            left_son = NULL;
            result->father=NULL;
            delete(this);
            return result;
        } else {
            result=right_son;
            right_son->father=NULL;
            right_son=NULL;
            delete(this);
            return result;
        }
    }
public:
    /**
     * constructor for an empty splay tree that gets a functor for comparison
     * @param comparator
     */
    explicit SplayTree(Comparator<T> &comparator) : comparator(comparator) {
        empty = true;
        left_son = NULL;
        right_son = NULL;
        father = NULL;
        data=NULL;
    }
    /**
     * a constructor for a splay tree that has an element in it.
     * @param comparator
     * @param value
     */
    SplayTree(Comparator<T> &comparator, T value) : comparator(comparator) {
        empty = false;
        data = value;
        left_son = NULL;
        right_son = NULL;
        father = NULL;
    }
    /**
     * recursive destructor.
     */
    ~SplayTree() {
        if (left_son != NULL) {
            delete (left_son);
        }
        if (right_son != NULL) {
            delete (right_son);
        }
    }
    /**
     * finds the smallest element in the tree according to the comparator supplied by the user.
     * @return the minimum.
     */
    T findMin() {
        if (!left_son) {
            return data;
        }
        return (left_son->findMin());
    }
    /**
     * finds the biggest element in the tree according to the comparator supplied by the user.
     * @return the minimum.
     */
    T findMax() {
        if (!right_son) {
            return data;
        }
        return (right_son->findMax());
    }


    /**
     * inserts an element to the tree.
     * @param element - the new element to add.
     */
    void insert(T element) {
        if (empty) {
            empty = false;
            data = element;
            return;
        } else {
            insertBinary(element);
            splay(element);
        }
    }

    /**
     * A function that tries to find a specific element in the SplayTree.
     * updates the SplayTree so that the root will be the found element or NULL.
     * @param element - the element to search for.
     * @return the SplayTree that holds the data we search for or NULL if the data does'nt exist.
     */
    T find(T element) {
        if (empty) {
            throw ElementDoesntExist();
        }
        splay(element);
        if (comparator(element, data)) {
            throw ElementDoesntExist();
        }
        return this->data;
    }

    /**
     * creates a sorted array of the elements in the tree.
     * @param array - the array to insert the values to.
     * @param index -
     */
    void sortedArray(T *array, int *index) {
        if (left_son) {
            left_son->sortedArray(array, index);
        }
        array[*index] = data;
        (*index)++;
        if (right_son) {
            right_son->sortedArray(array, index);
        }
    }

    void inOrderDel() {
        if (left_son) {
            left_son->inOrderDel();
        }
        if(data!=NULL) {
            delete (data);
        }
        if (right_son) {
            right_son->inOrderDel();
        }
    }
    /**
    * creates a line of nodes. the user should use concatLinear.
    * @param value
    * @return
    */
    SplayTree *concat(T value) {
        if (empty) {
            data = value;
            empty = false;
            return this;
        }
        SplayTree *new_node = new SplayTree(comparator, value);
        new_node->left_son = this;
        new_node->right_son=NULL;
        father=new_node;
        new_node->father=NULL;
        return new_node;
    }
    /**
     * in order scan of the elements in the tree.
     * @param operation - a functor to use on the elements.
     */
    void inOrder(Procedure<T> &operation) {
        if (empty) {
            return;
        }
        if (left_son) {
            left_son->inOrder(operation);
        }
        operation(data);
        if (right_son) {
            right_son->inOrder(operation);
        }
    }
    /**
     * removes an element from the tree.
     * @tparam S
     * @param s
     * @param elementToDelete
     */
    template<class S>
    friend void removeElement(SplayTree<S> **s, S elementToDelete);
};
template<class T>
void removeElement(SplayTree<T> **src, T elementToDelete) {
    *src = (*src)->kick(elementToDelete);
}

template<class T>
void concatLinear(SplayTree<T> **src, T val) {
    *src = (*src)->concat(val);
}

#endif //COLOSSEUM_SPLAYTREE_H