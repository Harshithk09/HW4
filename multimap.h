#ifndef MULTISET_H_
#define MULTISET_H_

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <map>
#include <stdexcept> // For std::runtime_error

template <typename K>
class Multiset {
 public:
  //
  // Public API
  // Constructor 

  Multiset() : root(nullptr), size(0) {}

  // Destructor
  ~Multiset() {
    Clear(root);
  }

  // * Capacity
  // Returns number of items in multiset --O(1)
  size_t Size() const {
    return size;
  }

  // Returns true if multiset is empty --O(1)
  bool Empty() const {
    return size == 0;
  }

  // * Modifiers
  // Inserts an item corresponding to @key in multiset --O(log N) on average
  void Insert(const K &key) {
    root = Insert(root, key);
    size++;
  }

  // Removes an item corresponding to @key from multiset --O(log N) on average
  // Throws exception if key doesn't exist
  void Remove(const K &key) {
    if (!Contains(key)) {
      throw std::runtime_error("No key");
    }
    root = Remove(root, key);
    size--;
  }

  // * Lookup
  // Return whether @key is found in multiset --O(log N) on average
  bool Contains(const K& key) const {
    return Contains(root, key) != nullptr;
  }

  // Returns number of items matching @key in multiset --O(log N) on average
  // Throws exception if key doesn't exist
  size_t Count(const K& key) const {
    Node* node = Contains(root, key);
    if (!node) {
      throw std::runtime_error("No key");
    }
    return node->count;
  }

  // Return greatest key less than or equal to @key --O(log N) on average
  // Throws exception if multiset is empty or no floor exists for key
  const K& Floor(const K &key) const {
    if (!root) {
      throw std::runtime_error("Empty multiset");
    }
    Node* floor_node = Floor(root, key);
    if (!floor_node) {
      throw std::runtime_error("No floor exists for key");
    }
    return floor_node->key;
  }

  // Return least key greater than or equal to @key --O(log N) on average
  // Throws exception if multiset is empty or no ceil exists for key
  const K& Ceil(const K &key) const {
    if (!node) {
        return 0; 
    }
    if (key == node->key) {
        return node->key;
    }
    if (key > node->key) {
        return Ceil(node->right, key);
    }
    unsigned int ceil = Ceil(node->left, key);
    return ceil ? ceil : node->key;
}

  // Return max key in multiset --O(log N) on average
  // Throws exception if multiset is empty
  const K& Max() const {
    if (!root) {
      throw std::runtime_error("Empty multiset");
    }
    return Max(root)->key;
  }

  // Return min key in multiset --O(log N) on average
  // Throws exception if multiset is empty
  const K& Min() const {
    if (!root) {
      throw std::runtime_error("Empty multiset");
    }
    return Min(root)->key;
  }

 private:
  // define Node struct as a pointer 
  struct Node {
    K key; // Key stored in the node
    int count; // Number of occurrences of the key
    Node* left; // Pointer to the left child
    Node* right; // Pointer to the right child

    // Constructor to initialize a node
    Node(const K& key) : key(key), count(1), left(nullptr), right(nullptr) {}
  };

  // root is the top of the tree
  Node* root;

  // size is the number of nodes in the tree
  size_t size;

  // How to insert a new node in the AVL Tree
  // you have to first see if the tree is empty, if it is then you insert the new node as the root
  // if the tree is not empty, you have to compare the new node to the root node
  // if the new node is less than the root node, you insert the new node to the left of the root node
  // if the new node is greater than the root node, you insert the new node to the right of the root node
  // if the new node is equal to the root node, you increment the count of the root node
  Node* Insert(Node* node, const K& key) {
    if (node == nullptr) {
      return new Node(key);
    }
    if (key < node->key) {
      node->left = Insert(node->left, key);
    } else if (key > node->key) {
      node->right = Insert(node->right, key);
    } else {
      node->count++; // Increment count if key already exists
    }
    return node;
  }

  // How to remove a node from the AVL Tree
  // you have to first see if the tree is empty, if it is then you throw an exception
  // if the tree is not empty, you have to compare the node to the root node
  // if the node is less than the root node, you remove the node from the left of the root node
  // if the node is greater than the root node, you remove the node from the right of the root node
  Node* Remove(Node* node, const K& key) {
    if (node == nullptr) {
      throw std::runtime_error("No key");
    }
    if (key < node->key) {
      node->left = Remove(node->left, key);
    } else if (key > node->key) {
      node->right = Remove(node->right, key);
    } else {
      if (node->count > 1) {
        node->count--; // Decrement count if key has multiple occurrences
      } else {
        if (node->left == nullptr) {
          Node* right = node->right;
          delete node;
          return right;
        }
        if (node->right == nullptr) {
          Node* left = node->left;
          delete node;
          return left;
        }
        Node* min = Min(node->right); // Find the minimum node in the right subtree
        node->key = min->key; // Replace the current node's key with the minimum key
        node->count = min->count; // Replace the current node's count with the minimum count
        node->right = Remove(node->right, min->key); // Remove the minimum node
      }
    }
    return node;
  }

  // How to find a node in the AVL Tree
  // you have to first see if the tree is empty, if it is then you return nullptr
  // if the tree is not empty, you have to compare the node to the root node
  // if the node is less than the root node, you find the node from the left of the root node
  // if the node is greater than the root node, you find the node from the right of the root node
  // you keep repeating this process until you find the node
  Node* Contains(Node* node, const K& key) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (key < node->key) {
      return Contains(node->left, key);
    } else if (key > node->key) {
      return Contains(node->right, key);
    } else {
      return node;
    }
  }

  // How to find the floor of a node in the AVL Tree
  // you have to first see if the tree is empty, if it is then you return nullptr
  // if the tree is not empty, you have to compare the node to the root node
  // if the node is less than the root node, you find the floor of the node from the left of the root node
  // if the node is greater than the root node, you find the floor of the node from the right of the root node
  // you keep repeating this process until you find the floor of the node
  Node* Floor(Node* node, const K& key) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (key == node->key) {
      return node;
    }
    if (key < node->key) {
      return Floor(node->left, key);
    }
    Node* floor = Floor(node->right, key);
    return floor ? floor : node;
  }

  // How to find the ceil of a node in the AVL Tree
  // you have to first see if the tree is empty, if it is then you return nullptr
  // if the tree is not empty, you have to compare the node to the root node
  // if the node is less than the root node, you find the ceil of the node from the left of the root node
  // if the node is greater than the root node, you find the ceil of the node from the right of the root node
  // you keep repeating this process until you find the ceil of the node
  Node* Ceil(Node* node, const K& key) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (key == node->key) {
      return node;
    }
    if (key > node->key) {
      return Ceil(node->right, key);
    }
    Node* ceil = Ceil(node->left, key);
    return ceil ? ceil : node;
  }

  // How to find the max of a node in the AVL Tree
  // see if the tree is empty, if it is then you return nullptr
  // if the tree is not empty, you find the max of the node from the right of the root node
  // you keep repeating this process until you find the max of the node
  Node* Max(Node* node) const {
    while (node->right) {
      node = node->right;
    }
    return node;
  }

  // How to find the min of a node in the AVL Tree
  // repeat the same process as max but from the left of the root node
  Node* Min(Node* node) const {
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  // to prevent memory leaks, you have to clear the tree
  // delete the left and right nodes of the root node
  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }
};

#endif  // MULTISET_H_