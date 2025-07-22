#include "AVLTree.h"
#include "User.h"

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

AVLTree::Friend* AVLTree::rotateRight(Friend* y) {
        Friend* x = y->left;
        Friend* Temp = x->right;
        x->right = y;
        y->left = Temp;
        newHeight(y);
        newHeight(x);
        return x;
    }

AVLTree::Friend* AVLTree::rotateLeft(Friend* x) {
        Friend* y = x->right;
        Friend* T2= y->left;
        y->left = x;
        x->right = T2;
        newHeight(x);
        newHeight(y);
        return y;
    }

AVLTree::Friend* AVLTree::balance(Friend* node){
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
AVLTree::Friend* AVLTree:: insert(Friend* node, const std::string& name) {
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

AVLTree::Friend* AVLTree::search(Friend* node, const std::string& name) const {
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
AVLTree::Friend* AVLTree::findMin(Friend* node) {
            while (node && node->left) {
                node = node->left;
            }
            return node;
        }
bool AVLTree::isfriend(const std::string& name) const {
            return search(root, name) != nullptr;
        }
AVLTree::Friend* AVLTree::removeFriend(Friend* root,const std::string& name) {
     if (!root) return root;
        if(name<root->name){
            root->left = removeFriend(root->left, name);
        }
        else if(name>root->name){
            root->right = removeFriend(root->right, name);
        }
        else{
            if(!root->left && !root->right){
                delete root;
                root = nullptr;
            }
            else if(!root->left){
                Friend* temp = root;
                root = root->right;
                delete temp;
            }
            else if(!root->right){
                Friend* temp = root;
                root = root->left;
                delete temp;
            }
            else{
                Friend* temp = findMin(root->right);
                root->name = temp->name;
                root->right = removeFriend(root->right, temp->name);
            }
        }

        if (!root) return root;
        newHeight(root);
        balance(root);

        if (balanceFactor(root) > 1 && balanceFactor(root->left) >= 0){
            return rotateRight(root);}
        
        if (balanceFactor(root)< -1 && balanceFactor(root->right) <= 0){
            return rotateLeft(root);}
        
        if (balanceFactor(root) > 1 && balanceFactor(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (balanceFactor(root) < -1 && balanceFactor(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }
        
        return root;
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
