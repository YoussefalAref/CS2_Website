#ifndef AVLTREE_H
#define AVLTREE_H

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T>
class AVLTree {
private:
    struct Friend{
        std::string name;
        
        Friend* left;
        Friend* right;
        int height;
        
        Friend(const std::string& n):name(n),left(nullptr), right(nullptr), height(1){}
    };
    
    Friend* root;
    public:

    int getHeight(Friend* node) const;
    int balanceFactor(Friend* node) const;
    void newHeight(Friend* node);
    Friend* rotateRight(Friend* y);
    Friend* rotateLeft(Friend* x);
    Friend* balance(Friend* node);
    AVLTree() : root(nullptr) {}
    ~AVLTree(){destroyTree(root);}
    void destroyTree(Friend* node);
    Friend* insert(Friend* node, const std::string& name);
    Friend* search(Friend* node, const std::string& name) const;
    void addFriend(const std::string& name);

    bool isfriend(const std::string& name) coonst;
    void displayFriends(Friend* node) const;
    void showAllFriends() const;
};

//     Friend* insertHelper(Friend* node, const string& name,int id){
//         if (node == nullptr) {
//             return new Friend(name,id);
//         }

//         if (data < node->data)
//             node->left = insertHelper(node->left, data);
//         else if(data > node->data)
//             node->right = insertHelper(node->right, data);
//         else
//             return node;

//         updateHeight(node);  // Fixed typo
        
//         int balance = getBalance(node);
        
//         // Left Left Case
//         if (balance > 1 && data < node->left->data) {
//             return rotateRight(node);
//         }
        
//         // Right Right Case
//         if (balance < -1 && data > node->right->data) {
//             return rotateLeft(node);
//         }
        
//         // Left Right Case
//         if (balance > 1 && data > node->left->data) {
//             node->left = rotateLeft(node->left);
//             return rotateRight(node);
//         }
        
//         // Right Left Case
//         if (balance < -1 && data < node->right->data) {
//             node->right = rotateRight(node->right);
//             return rotateLeft(node);
//         }
        
//         return node;
//     }

//     Node* findMin(Node* node){
//         while(node->left != nullptr)
//             node = node->left;
            
//         return node;
//     }

//     Node* removeHelper(Node* node, const T& data){
//         if(node == nullptr)
//             return node;

//         if (data < node->data){
//             node->left = removeHelper(node->left, data);
//         }
//         else if (data > node->data){
//             node->right = removeHelper(node->right, data);  // Added semicolon
//         }
//         else{
//             if(node->left == nullptr || node->right == nullptr){
//                 Node* temp = node->left ? node->left : node->right;  // Fixed syntax
//                 if(temp == nullptr){
//                     temp = node;
//                     node = nullptr;
//                 }else{
//                     *node = *temp;
//                 }
//                 delete temp;
//             }else{
//                 Node* temp = findMin(node->right);
//                 node->data = temp->data;
//                 node->right = removeHelper(node->right, temp->data);  // Added semicolon
//             }
//         }
        
//         if(node == nullptr){
//             return node;
//         }

//         updateHeight(node);

//         int balance = getBalance(node);

//         // Left Left Case
//         if (balance > 1 && getBalance(node->left) >= 0) {
//             return rotateRight(node);
//         }
        
//         // Left Right Case
//         if (balance > 1 && getBalance(node->left) < 0) {
//             node->left = rotateLeft(node->left);
//             return rotateRight(node);
//         }
        
//         // Right Right Case
//         if (balance < -1 && getBalance(node->right) <= 0) {
//             return rotateLeft(node);
//         }

//         // Right Left Case
//         if (balance < -1 && getBalance(node->right) > 0) {
//             node->right = rotateRight(node->right);
//             return rotateLeft(node);
//         }

//         return node;
//     }

//     bool searchHelper(Node* node, const T& data) const {
//         if (node == nullptr) {
//             return false;
//         }
        
//         if (data == node->data) {
//             return true;
//         } else if (data < node->data) {
//             return searchHelper(node->left, data);
//         } else {
//             return searchHelper(node->right, data);
//         }
//     }

//     void inorderHelper(Node* node, std::vector<T>& result) const {
//         if (node != nullptr) {
//             inorderHelper(node->left, result);
//             result.push_back(node->data);
//             inorderHelper(node->right, result);
//         }
//     }

//     void destroyTree(Node* node) {
//         if (node) {
//             destroyTree(node->left);
//             destroyTree(node->right);
//             delete node;
//         }
//     }

//     int sizeHelper(Node* node) const {
//         if (node == nullptr) {
//             return 0;
//         }
//         return 1 + sizeHelper(node->left) + sizeHelper(node->right);
//     }

// public:  // Fixed: lowercase 'public'
//     AVLTree() : root(nullptr) {}

//     ~AVLTree(){
//         destroyTree(root);  // Added semicolon
//     }

//     AVLTree(const AVLTree& other) : root(nullptr) {
//         *this = other;
//     }

//     AVLTree& operator=(const AVLTree& other) {
//         if (this != &other) {
//             destroyTree(root);
//             root = nullptr;
            
//             auto elements = other.getAllElements();
//             for (const auto& e
#endif
