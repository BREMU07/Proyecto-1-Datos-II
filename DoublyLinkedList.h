#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include "MPointer.h"

template<typename T>
class Node {
public:
    MPointer<T> data;
    MPointer<Node<T>> next;
    MPointer<Node<T>> prev;

    Node(T val) : data(MPointer<T>::New()), next(MPointer<Node<T>>()), prev(MPointer<Node<T>>()) {
        *data = val;
    }
};

template<typename T>
class DoublyLinkedList {
private:
    MPointer<Node<T>> head;
    MPointer<Node<T>> tail;

public:
    DoublyLinkedList() {}

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
            while (!(*current).next.isNull()) {
                if (*(*current).data > *(*(*current).next).data) {
                    std::swap(*(*current).data, *(*(*current).next).data);
                    swapped = true;
                }
                current = (*current).next;
            }
        } while (swapped);
    }
};

#endif
