#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <queue>  // Cola para gestionar índices libres

// Clase Gestora de Memoria
class MPointerGC {
private:
    std::vector<void*> memoryPool;  // "Memoria" simulada
    std::unordered_map<int, int> referenceCounts;  // Recuento de referencias por índice
    int currentIndex;  // Índice único y en incremento constante

    MPointerGC() : currentIndex(0) {}

public:
    // Método Singleton para obtener la instancia de MPointerGC
    static MPointerGC& getInstance() {
        static MPointerGC instance;
        return instance;
    }

    // Método para asignar memoria para un nuevo objeto
    int allocate(size_t size) {
        void* newMem = malloc(size);  // Simulamos la asignación de memoria
        if (!newMem) throw std::bad_alloc();

        int index = currentIndex++;  // Siempre incrementar el índice
        memoryPool.push_back(newMem);  // Agregar la memoria al pool
        referenceCounts[index] = 1;  // Nueva referencia inicial

        std::cout << "[MPointerGC] Memoria asignada en índice " << index << " de tamaño " << size << std::endl;
        return index;  // Devolvemos el índice asignado
    }

    // Método para liberar memoria, pero sin reutilizar el índice
    void deallocate(int index) {
        if (referenceCounts[index] == 0 && memoryPool[index] != nullptr) {
            free(memoryPool[index]);  // Liberar la memoria
            memoryPool[index] = nullptr;  // Marcar como liberada
            std::cout << "[MPointerGC] Memoria liberada en índice " << index << std::endl;
        } else {
            std::cout << "[MPointerGC] Intento de liberar memoria en índice con referencias activas o memoria nula " << index << std::endl;
        }
    }

    // Método para aumentar las referencias
    void addReference(int index) {
        referenceCounts[index]++;
        std::cout << "[MPointerGC] Referencia aumentada en índice " << index << " - Recuento: " << referenceCounts[index] << std::endl;
    }

    // **Método manual** para reducir las referencias, llamado por el usuario
    void removeReference(int index) {
        if (index == -1) return;  // No reducir referencias en punteros nulos

        if (referenceCounts.find(index) != referenceCounts.end() && referenceCounts[index] > 0) {
            referenceCounts[index]--;
            if (referenceCounts[index] == 0) {
                deallocate(index);  // Liberar memoria si el recuento llega a 0
            }
            std::cout << "[MPointerGC] Referencia reducida en índice " << index << " - Recuento: " << referenceCounts[index] << std::endl;
        } else {
            throw std::runtime_error("Error: Intento de reducir recuento de referencias inválido en índice " + std::to_string(index));
        }
    }

    // Método para obtener el puntero a la memoria en un índice dado
    void* getMemory(int index) {
        if (index >= memoryPool.size() || memoryPool[index] == nullptr) {
            throw std::runtime_error("[MPointerGC] Acceso a memoria inválida en índice " + std::to_string(index));
        }
        std::cout << "[MPointerGC] Accediendo a memoria en índice " << index << std::endl;
        return memoryPool[index];
    }
};
template<typename T>
class MPointer {
private:
    int memIndex;  // Índice en el "pool de memoria" del GC
    bool isNullPtr;  // Para simular un puntero nulo

    MPointer(int index) : memIndex(index), isNullPtr(false) {
        std::cout << "[MPointer] Creado MPointer en índice " << memIndex << std::endl;
    }

public:
    // Constructor por defecto (puntero nulo)
    MPointer() : memIndex(-1), isNullPtr(true) {
        std::cout << "[MPointer] Creado MPointer nulo" << std::endl;
    }

    // Método para crear un nuevo MPointer
    static MPointer<T> New() {
        int index = MPointerGC::getInstance().allocate(sizeof(T));  // Asignar memoria
        return MPointer<T>(index);
    }

    // Destructor (no reduce referencias automáticamente)
    ~MPointer() {
        std::cout << "[MPointer] Destructor llamado para índice " << memIndex << std::endl;
    }

    // Operador de desreferenciación
    T& operator*() {
        if (isNullPtr) throw std::runtime_error("Dereferencing null pointer");
        std::cout << "[MPointer] Desreferenciando índice " << memIndex << std::endl;
        return *(T*)(MPointerGC::getInstance().getMemory(memIndex));  // Acceso a la memoria
    }

    // Operador de asignación entre MPointers (no reduce referencias automáticamente)
    MPointer<T>& operator=(const MPointer<T>& other) {
        if (this != &other) {
            memIndex = other.memIndex;
            isNullPtr = other.isNullPtr;
            std::cout << "[MPointer] Asignado MPointer con índice " << memIndex << std::endl;
        }
        return *this;
    }

    // Verificar si es un puntero nulo
    bool isNull() const {
        return isNullPtr;
    }

    // Método para obtener el índice (por si el usuario necesita usar removeReference)
    int getIndex() const {
        return memIndex;
    }
};
#endif