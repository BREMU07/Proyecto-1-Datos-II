#include "gtest/gtest.h"
#include "MPointer.h"

// Clase de Fixture de Prueba para MPointerGC
class MPointerGCTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Iniciar el Recolector de Basura antes de cada prueba
        MPointerGC::getInstance().startGC(1);
    }

    void TearDown() override {
        // Detener el Recolector de Basura después de cada prueba
        MPointerGC::getInstance().stopGC();
    }
};

// Prueba si el Recolector de Basura se inicia y detiene correctamente
TEST_F(MPointerGCTest, TestStartStopGC) {
    MPointerGC& gc = MPointerGC::getInstance();
    EXPECT_TRUE(gc.isRunning()); // Después de SetUp, el GC debería estar en ejecución
    gc.stopGC();
    EXPECT_FALSE(gc.isRunning()); // Después de stopGC, el GC no debería estar en ejecución
}

// Prueba de asignación de memoria en MPointerGC
TEST_F(MPointerGCTest, TestMemoryAllocation) {
    MPointerGC& gc = MPointerGC::getInstance();
    
    int index = gc.allocate(sizeof(int));
    EXPECT_NE(index, -1); // La asignación debería dar un índice válido
}

// Prueba de liberación de memoria en MPointerGC
TEST_F(MPointerGCTest, TestMemoryDeallocation) {
    MPointerGC& gc = MPointerGC::getInstance();
    
    int index = gc.allocate(sizeof(int));
    gc.removeReference(index); // La eliminación de la referencia debería liberar la memoria
    EXPECT_THROW(gc.getMemory(index), std::runtime_error); // Intentar acceder a la memoria liberada debería lanzar una excepción
}

// Prueba de creación y desreferenciación de MPointer
TEST_F(MPointerGCTest, TestMPointerNew) {
    MPointer<int> ptr = MPointer<int>::New();
    *ptr = 42;
    EXPECT_EQ(*ptr, 42); // Prueba de desreferenciación y asignación de valor
}

// Prueba de asignación de MPointer y conteo de referencias
TEST_F(MPointerGCTest, TestMPointerAssignment) {
    MPointer<int> ptr1 = MPointer<int>::New();
    *ptr1 = 10;

    MPointer<int> ptr2;
    ptr2 = ptr1; // Prueba del operador de asignación
    EXPECT_EQ(*ptr2, 10); // ptr2 debería apuntar ahora a la misma memoria que ptr1
}

// Prueba de que la desreferenciación de un puntero nulo lance una excepción
TEST_F(MPointerGCTest, TestDereferencingNullPointer) {
    MPointer<int> ptr;
    EXPECT_THROW(*ptr, std::runtime_error); // La desreferenciación de un puntero nulo debería lanzar una excepción
}