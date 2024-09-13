#include <gtest/gtest.h>
#include "DoublyLinkedList.h"

class DoublyLinkedListTest : public ::testing::Test {
protected:
    DoublyLinkedList<int> list;

    void SetUp() override {
        MPointerGC::getInstance().startGC(1);
        // Inicializar una lista con 3 valores
        list.append(3);
        list.append(1);
        list.append(2);
    }

    void TearDown() override {
        // Limpiar después de cada prueba
        list.removeReferences();
    }
};

// Prueba de agregar elementos e imprimir la lista
TEST_F(DoublyLinkedListTest, AppendAndPrintList) {
    MPointerGC::getInstance().startGC(1);
    testing::internal::CaptureStdout();
    list.printList();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "3\n1\n2\n\n");
}

// Prueba de la funcionalidad de BubbleSort
TEST_F(DoublyLinkedListTest, BubbleSortTest) {
    MPointerGC::getInstance().startGC(1);
    list.bubbleSort();
    testing::internal::CaptureStdout();
    list.printList();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1\n2\n3\n\n");
}

// Prueba de la funcionalidad de QuickSort
TEST_F(DoublyLinkedListTest, QuickSortTest) {
    MPointerGC::getInstance().startGC(1);
    list.quickSort();
    testing::internal::CaptureStdout();
    list.printList();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1\n2\n3\n\n");
}

// Prueba de la funcionalidad de InsertionSort
TEST_F(DoublyLinkedListTest, InsertionSortTest) {
    MPointerGC::getInstance().startGC(1);
    list.insertionSort();
    testing::internal::CaptureStdout();
    list.printList();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1\n2\n3\n\n");
}

// Prueba de eliminar referencias
TEST_F(DoublyLinkedListTest, RemoveReferencesTest) {
    list.removeReferences();
    testing::internal::CaptureStdout();
    list.printList();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");  // Espera que no haya salida después de eliminar las referencias
}
