#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include <stack>
#include "MPointer.h"

using namespace std;

template<typename T>
class Node {
public:
    MPointer<T> data;
    MPointer<Node<T>> next;
    MPointer<Node<T>> prev;

    Node(T val) : data(MPointer<T>::New()), next(MPointer<Node<T>>()), prev(MPointer<Node<T>>()) {
        *data = val;
        cout << "[Node] Nodo creado con valor: " << val << endl;
    }
};

template<typename T>
class DoublyLinkedList {
private:
    MPointer<Node<T>> head;
    MPointer<Node<T>> tail;

public:
    void append(T value) {
        MPointer<Node<T>> newNode = MPointer<Node<T>>::New();
        *newNode = Node<T>(value);

        if (head.isNull()) {
            head = newNode;
            tail = newNode;
        } else {
            (*tail).next = newNode;
            (*newNode).prev = tail;
            tail = newNode;
        }
    }

    void printList() {
        MPointer<Node<T>> temp = head;
        if (temp.isNull()) {
            return;
        }
        while (!temp.isNull()) {
            cout << *(*temp).data << endl;
            temp = (*temp).next;
        }
        cout << endl;
    }

    void removeReferences() {
        MPointer<Node<T>> temp = head;
        while (!temp.isNull()) {
            int index = temp.getIndex();
            MPointer<Node<T>> nextNode = (*temp).next; // Guardar el siguiente nodo antes de desasignar
            MPointerGC::getInstance().removeReference(index);
            temp = nextNode; // Mover al siguiente nodo
        }
    }

    void bubbleSort() {
        if (head.isNull() || (*head).next.isNull()) {
            cout << "La lista está vacía o solo tiene un nodo. No se necesita ordenar." << endl;
            return;
        }

        bool swapped;
        do {
            swapped = false;
            MPointer<Node<T>> current = head;
            while (!(*current).next.isNull()) {
                MPointer<Node<T>> nextNode = (*current).next;
                if (*(*current).data > *(*nextNode).data) {
                    T temp = *(*current).data;
                    *(*current).data = *(*nextNode).data;
                    *(*nextNode).data = temp;
                    swapped = true;
                }
                current = (*current).next;
            }

        } while (swapped);
    }

    MPointer<Node<T>> partition(MPointer<Node<T>> low, MPointer<Node<T>> high) {
        T pivotValue = *(*high).data;
        MPointer<Node<T>> i = (*low).prev;

        for (MPointer<Node<T>> j = low; j.getIndex() != high.getIndex(); j = (*j).next) {
            if (*(*j).data < pivotValue) {
                i = (i.isNull()) ? low : (*i).next;
                T temp = *(*i).data;
                *(*i).data = *(*j).data;
                *(*j).data = temp;
            }
        }

        i = (i.isNull()) ? low : (*i).next;
        T temp = *(*i).data;
        *(*i).data = *(*high).data;
        *(*high).data = temp;

        return i;
    }

    void insertionSort() {
        if (head.isNull() || (*head).next.isNull()) {
            return;
        }

        MPointer<Node<T>> current = (*head).next;

        while (!current.isNull()) {
            T currentValue = *(*current).data;
            MPointer<Node<T>> previous = (*current).prev;

            while (!previous.isNull() && *(*previous).data > currentValue) {
                *(*(*previous).next).data = *(*previous).data;
                previous = (*previous).prev;
            }

            if (previous.isNull()) {
                *(*head).data = currentValue;
            } else {
                *(*(*previous).next).data = currentValue;
            }

            current = (*current).next;
        }
    }

    void quickSort() {
        if (head.isNull() || (*head).next.isNull()) return;

        stack<pair<MPointer<Node<T>>, MPointer<Node<T>>>> stack;
        stack.push({head, tail});

        while (!stack.empty()) {
            auto [low, high] = stack.top();
            stack.pop();

            MPointer<Node<T>> pivot = partition(low, high);

            if (pivot.getIndex() != low.getIndex() && (*pivot).prev.getIndex() != low.getIndex()) {
                stack.push({low, (*pivot).prev});
            }

            if (pivot.getIndex() != high.getIndex() && (*pivot).next.getIndex() != high.getIndex()) {
                stack.push({(*pivot).next, high});
            }
        }
    }
};
#endif