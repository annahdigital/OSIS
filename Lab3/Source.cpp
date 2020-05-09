#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;


Node* findFreeNode(int value, Node* parent) {
    Node* tmp = (Node*)malloc(sizeof(Node));
    tmp->left = tmp->right = NULL;
    tmp->data = value;
    tmp->parent = parent;
    return tmp;
}


void insertItem(Node** head, int value) {
    Node* tmp = NULL;
    Node* ins = NULL;
    if (*head == NULL) {
        *head = findFreeNode(value, NULL);
        return;
    }
    tmp = *head;
    while (tmp) {
        if (value > tmp->data) {
            if (tmp->right) {
                tmp = tmp->right;
                continue;
            }
            else {
                tmp->right = findFreeNode(value, tmp);
                return;
            }
        }
        else if (value < tmp->data) {
            if (tmp->left) {
                tmp = tmp->left;
                continue;
            }
            else {
                tmp->left = findFreeNode(value, tmp);
                return;
            }
        }
        else {
            exit(2);
        }
    }
}


Node* getMinNode(Node* root) {
    while (root->left) {
        root = root->left;
    }
    return root;
}


Node* getMaxNode(Node* root) {
    while (root->right) {
        root = root->right;
    }
    return root;
}


Node* getNodeByValue(Node* root, int value) {
    while (root) {
        if (root->data > value) {
            root = root->left;
            continue;
        }
        else if (root->data < value) {
            root = root->right;
            continue;
        }
        else {
            return root;
        }
    }
    return NULL;
}

void removeNodeByPtr(Node* target) {
    if (target->left && target->right) {
        Node* localMax = getMaxNode(target->left);
        target->data = localMax->data;
        removeNodeByPtr(localMax);
        return;
    }
    else if (target->left) {
        if (target == target->parent->left) {
            target->parent->left = target->left;
            target->left->parent = target->parent;
        }
        else {
            target->parent->right = target->left;
            target->left->parent = target->parent;
        }
    }
    else if (target->right) {
        if (target == target->parent->right) {
            target->parent->right = target->right;
            target->right->parent = target->parent;
        }
        else {
            target->parent->left = target->right;
            target->right->parent = target->parent;
        }
    }
    else {
        if (target == target->parent->left) {
            target->parent->left = NULL;
        }
        else {
            target->parent->right = NULL;
        }
    }
    free(target);
}


void removeItem(Node* root, int value) {
    Node* target = getNodeByValue(root, value);
    removeNodeByPtr(target);
}


void printTree(Node* root)
{
    if (root) {
        printf("%d ", root->data);
        printTree(root->left);
        printTree(root->right);
    }
}

void deleteAllFromTheThree(Node* root)
{
    if (root) {
        deleteAllFromTheThree(root->left);
        deleteAllFromTheThree(root->right);
        if (root->parent){
            root->parent->right = NULL;
            root->parent->left = NULL;
        }
        free(root);
    }
}


void main() {
    std::cout << "Initial tree: " << std::endl;
    Node* root = NULL;
    insertItem(&root, 10);
    insertItem(&root, 12);
    insertItem(&root, 8);
    insertItem(&root, 9);
    insertItem(&root, 7);
    insertItem(&root, 4);
    insertItem(&root, 3);
    printTree(root);
    removeItem(root, 4);
    removeItem(root, 8);
    removeItem(root, 3);
    std::cout << std::endl << "New tree: " << std::endl;
    printTree(root);
    deleteAllFromTheThree(root);
    std::cout << std::endl << "After deleting all: " << std::endl;
    printTree(root);

}