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
    std::queue<int> freeIndices;  // Cola de índices libres
    int currentIndex;  // Índice de la próxima memoria libre

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

        int index;
        if (!freeIndices.empty()) {
            // Reutilizar un índice libre si existe
            index = freeIndices.front();
            freeIndices.pop();
            memoryPool[index] = newMem;
        } else {
            // Crear un nuevo índice
            index = currentIndex++;
            memoryPool.push_back(newMem);
        }

        referenceCounts[index] = 1;  // Nueva referencia inicial
        std::cout << "[MPointerGC] Memoria asignada en índice " << index << " de tamaño " << size << std::endl;
        return index;  // Devolvemos el índice asignado
    }

    // Método para liberar memoria
    void deallocate(int index) {
        if (referenceCounts[index] == 0) {
            free(memoryPool[index]);  // Liberar la memoria
            memoryPool[index] = nullptr;  // Marcar como liberada
            freeIndices.push(index);  // Agregar a la lista de índices libres
            std::cout << "[MPointerGC] Memoria liberada en índice " << index << std::endl;
        } else {
            std::cout << "[MPointerGC] Intento de liberar memoria con referencias en índice " << index << std::endl;
        }
    }


    // Método para aumentar las referencias
    void addReference(int index) {
        referenceCounts[index]++;
        std::cout << "[MPointerGC] Referencia aumentada en índice " << index << " - Recuento: " << referenceCounts[index] << std::endl;
    }

    // Método para reducir las referencias y liberar si es necesario
    void removeReference(int index) {
        if (index == -1) return;  // No reducir referencias en punteros nulos
        if (--referenceCounts[index] == 0) {
            deallocate(index);  // Liberar memoria si el recuento llega a 0
        } else if (referenceCounts[index] < 0) {
            throw std::runtime_error("Error: Recuento de referencias negativo en índice " + std::to_string(index));
        }
        std::cout << "[MPointerGC] Referencia reducida en índice " << index << " - Recuento: " << referenceCounts[index] << std::endl;
    }


    // Método para obtener el puntero a la memoria en un índice dado
    void* getMemory(int index) {
        if (index >= memoryPool.size() || memoryPool[index] == nullptr) {
            throw std::runtime_error("[MPointerGC] Acceso a memoria inválida en índice " + std::to_string(index));
        }
        std::cout << "[MPointerGC] memoryPool[" << index << "] = " << memoryPool[index] << std::endl;
        return memoryPool[index];
    }
};

// Implementación de MPointer
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

    // Destructor
    ~MPointer() {
        if (!isNullPtr) {
            MPointerGC::getInstance().removeReference(memIndex);  // Remover referencia
        }
        std::cout << "[MPointer] Destructor llamado para índice " << memIndex << std::endl;
    }

    // Operador de desreferenciación
    T& operator*() {
        if (isNullPtr) {
            throw std::runtime_error("Desreferenciando un puntero nulo");
        }
        std::cout << "[MPointer] Desreferenciando índice " << memIndex << std::endl;
        return *(T*)(MPointerGC::getInstance().getMemory(memIndex));
    }

    // Operador de asignación entre MPointers
    MPointer<T>& operator=(const MPointer<T>& other) {
        if (this != &other) {
            if (!isNullPtr) {
                MPointerGC::getInstance().removeReference(memIndex);  // Liberar referencia actual
            }
            memIndex = other.memIndex;
            isNullPtr = other.isNullPtr;
            if (!isNullPtr) {
                MPointerGC::getInstance().addReference(memIndex);  // Aumentar referencia en el nuevo
            }
        }
        std::cout << "[MPointer] Asignado MPointer con índice " << memIndex << std::endl;
        return *this;
    }


    // Verificar si es un puntero nulo
    bool isNull() const {
        return isNullPtr;
    }
};
#endif