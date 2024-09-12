#include "DoublyLinkedList.h"

using namespace std;

int main() {
    MPointerGC::getInstance().startGC(1);

    DoublyLinkedList<int> listBubble;
    DoublyLinkedList<int> listQuick;
    DoublyLinkedList<int> listInsertion;

    // Agregar nodos a la lista
    listBubble.append(30);
    listBubble.append(10);
    listBubble.append(20);
    listBubble.append(3);
    listBubble.append(8);
    listBubble.append(67);
    listBubble.append(18);
    listBubble.append(90);

    listQuick.append(10);
    listQuick.append(6);
    listQuick.append(89);
    listQuick.append(500);
    listQuick.append(1);
    listQuick.append(4);
    listQuick.append(25);
    listQuick.append(67);

    listInsertion.append(56);
    listInsertion.append(56);
    listInsertion.append(1);
    listInsertion.append(89);
    listInsertion.append(34);
    listInsertion.append(7);
    listInsertion.append(8);
    listInsertion.append(2);

    // Imprimir la lista
    cout << "Lista Bubble: " << endl;
    listBubble.printList();

    cout <<"Lista Quick: " << endl;
    listQuick.printList();

    cout << "Lista Insertion: " << endl;
    listInsertion.printList();

    listQuick.quickSort();
    listBubble.bubbleSort();
    listInsertion.insertionSort();

    cout << "Lista Bubble ordenada " << endl;
    listBubble.printList();

    cout << "Lista Quick ordenada " << endl;
    listQuick.printList();

    cout << "Lista Insertion ordenada " << endl;
    listInsertion.printList();

    listBubble.removeReferences();  // Eliminar las referencias manualmente para liberar la memoria
    listQuick.removeReferences();
    listInsertion.removeReferences();

    return 0;
}
