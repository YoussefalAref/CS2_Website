#ifndef AVLTREE_H
#define AVLTREE_H

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T>
class AVLTree {
private:
    struct Node{
        T data;
        Node* left;
        Node* right;
        int height;

        Node(const T& val): data(val), left(nullptr), right(nullptr), height(1){}
    };

    Node* root;

    int getHeight(Node* node) const{
        return node ? node->height : 0;
    }

    int getBalance(Node* node) const{
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(Node* node){
        if(node)
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* Temp = x->right;

        x->right = y;
        y->left = Temp;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* insertHelper(Node* node, const T& data){
        if (node == nullptr) {
            return new Node(data);
        }

        if (data < node->data)
            node->left = insertHelper(node->left, data);
        else if(data > node->data)
            node->right = insertHelper(node->right, data);
        else
            return node;

        updateHeight(node);  // Fixed typo
        
        int balance = getBalance(node);
        
        // Left Left Case
        if (balance > 1 && data < node->left->data) {
            return rotateRight(node);
        }
        
        // Right Right Case
        if (balance < -1 && data > node->right->data) {
            return rotateLeft(node);
        }
        
        // Left Right Case
        if (balance > 1 && data > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right Left Case
        if (balance < -1 && data < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }

    Node* findMin(Node* node){
        while(node->left != nullptr)
            node = node->left;
            
        return node;
    }

    Node* removeHelper(Node* node, const T& data){
        if(node == nullptr)
            return node;

        if (data < node->data){
            node->left = removeHelper(node->left, data);
        }
        else if (data > node->data){
            node->right = removeHelper(node->right, data);  // Added semicolon
        }
        else{
            if(node->left == nullptr || node->right == nullptr){
                Node* temp = node->left ? node->left : node->right;  // Fixed syntax
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                }else{
                    *node = *temp;
                }
                delete temp;
            }else{
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = removeHelper(node->right, temp->data);  // Added semicolon
            }
        }
        
        if(node == nullptr){
            return node;
        }

        updateHeight(node);

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        
        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    bool searchHelper(Node* node, const T& data) const {
        if (node == nullptr) {
            return false;
        }
        
        if (data == node->data) {
            return true;
        } else if (data < node->data) {
            return searchHelper(node->left, data);
        } else {
            return searchHelper(node->right, data);
        }
    }

    void inorderHelper(Node* node, std::vector<T>& result) const {
        if (node != nullptr) {
            inorderHelper(node->left, result);
            result.push_back(node->data);
            inorderHelper(node->right, result);
        }
    }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    int sizeHelper(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + sizeHelper(node->left) + sizeHelper(node->right);
    }

public:  // Fixed: lowercase 'public'
    AVLTree() : root(nullptr) {}

    ~AVLTree(){
        destroyTree(root);  // Added semicolon
    }

    AVLTree(const AVLTree& other) : root(nullptr) {
        *this = other;
    }

    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            destroyTree(root);
            root = nullptr;
            
            auto elements = other.getAllElements();
            for (const auto& elem : elements) {
                insert(elem);
            }
        }
        return *this;
    }

    void insert(const T& data) {
        root = insertHelper(root, data);
    }
    
    void remove(const T& data) {
        root = removeHelper(root, data);
    }
    
    bool search(const T& data) const {
        return searchHelper(root, data);
    }
    
    bool isEmpty() const {
        return root == nullptr;
    }
    
    int size() const {
        return sizeHelper(root);
    }
    
    int height() const {
        return getHeight(root);
    }
    
    std::vector<T> inorderTraversal() const {
        std::vector<T> result;
        inorderHelper(root, result);
        return result;
    }
    
    std::vector<T> getAllElements() const {
        return inorderTraversal();
    }
    
    std::vector<T> levelorderTraversal() const {
        std::vector<T> result;
        if (root == nullptr) return result;
        
        std::vector<Node*> queue;
        queue.push_back(root);
        
        while (!queue.empty()) {
            Node* current = queue.front();
            queue.erase(queue.begin());
            
            result.push_back(current->data);
            
            if (current->left) queue.push_back(current->left);
            if (current->right) queue.push_back(current->right);
        }
        
        return result;
    }
};

#endif
