#include "DoublyLinkedList.h"

int main() {
    try {
        DoublyLinkedList<int> list;

        list.append(4);
        list.append(2);
        list.append(3);
        list.append(1);
        
        //int size = list.getSize();
        //std::cout << "Tamaño de la lista: " << size << std::endl;
        std::cout << "Lista antes de ordenar: ";
        list.printList();

        //list.bubbleSort();

        std::cout << "Lista después de ordenar: ";
        list.printList();
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
