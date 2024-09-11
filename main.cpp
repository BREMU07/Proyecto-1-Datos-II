#include "DoublyLinkedList.h"

using namespace std;

int main() {
    DoublyLinkedList<int> list;

    // Agregar nodos a la lista
    list.append(30);
    list.append(10);
    list.append(20);
    list.append(3);
    list.append(8);
    list.append(67);
    list.append(18);
    list.append(90);

    // Imprimir la lista
    cout << "Lista: " << endl;
    list.printList();

     
    list.insertionSort();

    cout << "Lista ordenada " << endl;
    list.printList();


    // El usuario debe reducir manualmente las referencias cuando ya no necesite los nodos
    //list.removeReferences();  // Eliminar las referencias manualmente para liberar la memoria

    return 0;
}
