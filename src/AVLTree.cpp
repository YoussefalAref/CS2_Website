#ifndef AVLTREE_H
#define AVLTREE_H
#include "AVLTree.h"

int AVLTree::getHeight(Friend* node) const{
        return (node ? node->height : 0);
}

int AVLTree::balanceFactor(Friend* node) const{
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

void AVLTree::newHeight(Friend* node){
        if(node)
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

Friend* AVLTree:: rotateRight(Friend* y) {
        Friend* x = y->left;
        Friend* Temp = x->right;
        x->right = y;
        y->left = Temp;
        newHeight(y);
        newHeight(x);
        return x;
    }

Friend* AVLTree::rotateLeft(Friend* x) {
        Friend* y = x->right;
        Friend* T2= y->left;
        y->left = x;
        x->right = T2;
        newHeight(x);
        newHeight(y);
        return y;
    }

Friend* AVLTree::balance(Friend* node){
        if(!node) return nullptr;

        newHeight(node);
        int bf = balanceFactor(node);
        if(bf > 1) {
            if(balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        else if(bf < -1) {
            if(balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);            
        }
        return node;
    }
void AVLTree::destroyTree(Friend* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
Friend* AVLTree:: insert(Friend* node, const std::string& name) {
        if (!node) {
            return new Friend(name);
        }

        if (name < node->name) {
            node->left = insert(node->left, name);
        } else if (name > node->name) {
            node->right = insert(node->right, name);
        } else {
            return node; // Duplicate names are not allowed
        }
        return balance(node);
    }   

Friend* AVLTree::Friend* node, const std::string& name) const {
        if (!node || node->name == name) {
            return node;
        }
        if (name < node->name) {
            return search(node->left, name);
        } else {
            return search(node->right, name);
        }
    }

void AVLTree::addFriend(const std::string& name) {
        root = insert(root, name);
    }

bool AVLTree::isfriend(const std::string& name) const {
        return search(root, name) != nullptr;
    }

void AVLTree::displayFriends(Friend* node) const {
        if (node) {
            displayFriends(node->left);
            std::cout << "Friend: " << node->name << std::endl;
            displayFriends(node->right);
        }
    }

void AVLTree::showAllFriends() const {
        displayFriends(root);
    }
#endif