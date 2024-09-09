#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include "MPointer.h"

template<typename T>
class Node {
public:
    MPointer<T> data;  // El dato almacenado
    MPointer<Node<T>> next;  // Puntero al siguiente nodo
    MPointer<Node<T>> prev;  // Puntero al nodo anterior

    Node(T val) : data(MPointer<T>::New()), next(), prev() {
        *data = val;
        std::cout << "[Node] Nodo creado con valor: " << val << std::endl;
    }
        
};


template<typename T>
class DoublyLinkedList {
private:
    MPointer<Node<T>> head;
    MPointer<Node<T>> tail;

public:
    DoublyLinkedList() : head(), tail() {}

    void append(T value) {
        MPointer<Node<T>> newNode = MPointer<Node<T>>::New();
        *newNode = Node<T>(value);

        if (head.isNull()) {
            head = newNode;
            tail = head;
        } else {
            (*tail).next = newNode;
            (*newNode).prev = tail;
            tail = newNode;
        }
    }

    void printList() {
        MPointer<Node<T>> temp = head;
        while (!temp.isNull()) {
            std::cout << *(*temp).data << " ";
            temp = (*temp).next;
        }
        std::cout << std::endl;
    }

    void bubbleSort() {
        if (head.isNull()) return;
        bool swapped;
        do {
            swapped = false;
            MPointer<Node<T>> current = head;
            while (!current.isNull() && !(*current).next.isNull()) {
                MPointer<Node<T>> nextNode = (*current).next;
                if (*(*current).data > *(*nextNode).data) {
                    std::swap(*(*current).data, *(*nextNode).data);
                    swapped = true;
                }
                current = nextNode;
            }
        } while (swapped);
    }
};


#endif
