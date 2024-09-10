#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include "MPointer.h"

template<typename T>
class Node {
public:
    MPointer<T> data;  // Dato almacenado en el nodo
    MPointer<Node<T>> next;  // Puntero al siguiente nodo
    MPointer<Node<T>> prev;  // Puntero al nodo anterior

    // Constructor del nodo
    Node(T val) : data(MPointer<T>::New()), next(MPointer<Node<T>>()), prev(MPointer<Node<T>>()) {
        *data = val;  // Asignar el valor al puntero de datos
        std::cout << "[Node] Nodo creado con valor: " << val << std::endl;
    }
};

template<typename T>
class DoublyLinkedList {
private:
    MPointer<Node<T>> head;  // Puntero al primer nodo
    MPointer<Node<T>> tail;  // Puntero al último nodo

public:
    // Constructor de la lista doblemente enlazada
    void append(T value) {
        // Crear un nuevo nodo
        MPointer<Node<T>> newNode = MPointer<Node<T>>::New();
        *newNode = Node<T>(value);  // Asignar el valor al nodo

        std::cout << "[DoublyLinkedList] Nodo creado con valor: " << value << std::endl;

        // Si la lista está vacía
        if (head.isNull()) {
            head = newNode;
            tail = newNode;
            std::cout << "[DoublyLinkedList] Este es el primer nodo. Head y Tail apuntan al nodo con valor: " << value << std::endl;
        } else {
            // Enlazar el nuevo nodo al final
            (*tail).next = newNode;  // Asignar nuevo nodo como el siguiente del tail actual
            (*newNode).prev = tail;  // Asignar tail actual como anterior del nuevo nodo
            tail = newNode;          // Actualizar tail

            std::cout << "[DoublyLinkedList] Nodo agregado al final. Nuevo Tail con valor: " << value << std::endl;
        }
    }
    // Método para imprimir los valores de la lista
    void printList() {
        MPointer<Node<T>> temp = head;
        // Depuración para verificar si el head es nulo
        if (temp.isNull()) {
            std::cout << "[DoublyLinkedList] El puntero head es nulo, la lista está vacía." << std::endl;
            return;
        }
        std::cout << "[DoublyLinkedList] Recorriendo la lista desde head." << std::endl;
        // Recorremos los nodos y añadimos mensajes de depuración
        while (!temp.isNull()) {
            std::cout << "[DoublyLinkedList] Nodo con valor: " << *(*temp).data << std::endl;  // Desreferenciar el valor del nodo
            temp = (*temp).next;  // Ir al siguiente nodo
        // Verificar si el siguiente puntero es nulo
            if (temp.isNull()) {
                std::cout << "[DoublyLinkedList] No hay más nodos en la lista." << std::endl;
            }
        }
        std::cout << std::endl;
    }
    // Método para reducir manualmente las referencias de todos los nodos de la lista
    void removeReferences() {
        MPointer<Node<T>> temp = head;
        while (!temp.isNull()) {
            int index = temp.getIndex();  // Obtener el índice del nodo actual
            std::cout << index << std::endl;
            MPointerGC::getInstance().removeReference(index);  // Eliminar referencia al nodo actual
            temp = (*temp).next;  // Ir al siguiente nodo
        }
    }

    void bubbleSort() {
        // Si la lista está vacía o tiene solo un elemento, no es necesario ordenar
        if (head.isNull() || (*head).next.isNull()) {
            std::cout << "[BubbleSort] La lista está vacía o tiene solo un nodo. No se necesita ordenación." << std::endl;
            return;
        }

        bool swapped;   
        do {
            swapped = false;
            MPointer<Node<T>> current = head;

            // Iterar por la lista mientras existan más nodos
            while (!(*current).next.isNull()) {
                MPointer<Node<T>> nextNode = (*current).next;

                // Comparar los valores de los nodos actuales y siguientes
                if (*(*current).data > *(*nextNode).data) {
                    // Intercambiar los valores de los datos entre los nodos
                    T temp = *(*current).data;
                    *(*current).data = *(*nextNode).data;
                    *(*nextNode).data = temp;

                    swapped = true;
                }

                // Avanzar al siguiente nodo
                current = (*current).next;
            }

        } while (swapped);  // Repetir hasta que no haya más intercambios
    }

};
#endif
