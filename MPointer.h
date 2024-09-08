#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

// Definición de la clase MPointerGC
class MPointerGC {
private:
    std::unordered_map<void*, int> referenceCounts;
    std::mutex gcMutex;
    std::atomic<bool> running;
    std::thread gcThread;

    // Constructor privado para el patrón Singleton
    MPointerGC() : running(true), gcThread(&MPointerGC::collectGarbage, this) {}

    // Función que realiza la recolección de basura
    void collectGarbage() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            std::lock_guard<std::mutex> lock(gcMutex);
            for (auto it = referenceCounts.begin(); it != referenceCounts.end();) {
                if (it->second == 0) {
                    free(it->first);
                    it = referenceCounts.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

public:
    // Función estática que retorna la única instancia de MPointerGC
    static MPointerGC& getInstance() {
        static MPointerGC instance;
        return instance;
    }

    // Prohíbe la copia y asignación de la instancia
    MPointerGC(const MPointerGC&) = delete;
    MPointerGC& operator=(const MPointerGC&) = delete;

    // Funciones para manejar las referencias de punteros
    void addReference(void* ptr) {
        std::lock_guard<std::mutex> lock(gcMutex);
        referenceCounts[ptr]++;
    }

    void removeReference(void* ptr) {
        std::lock_guard<std::mutex> lock(gcMutex);
        if (--referenceCounts[ptr] == 0) {
            free(ptr);
            referenceCounts.erase(ptr);
        }
    }

    // Función para detener el recolector de basura
    void stopGC() {
        running = false;
        if (gcThread.joinable()) {
            gcThread.join();
        }
    }

    // Destructor
    ~MPointerGC() {
        stopGC();
    }
};

// Definición de la clase MPointer
template<typename T>
class MPointer {
private:
    std::shared_ptr<T> ptr;

    // Constructor privado, sólo accesible a través de MPointer::New
    MPointer(T* ptr) : ptr(ptr, [](T* p) { free(p); }) {
        MPointerGC::getInstance().addReference(ptr);
    }

public:
    // Constructor por defecto
    MPointer() : ptr(nullptr) {}

    // Método para crear un nuevo MPointer
    static MPointer<T> New() {
        T* rawPtr = (T*)malloc(sizeof(T));
        if (!rawPtr) throw std::bad_alloc();  // Verifica que malloc no falle
        return MPointer<T>(rawPtr);
    }

    // Destructor
    ~MPointer() {
        reset();  // Llama a reset para liberar cualquier recurso
    }

    // Método para reiniciar el puntero
    void reset() {
        if (ptr) {
            MPointerGC::getInstance().removeReference(ptr.get());
            ptr.reset();
        }
    }

    // Sobrecarga de operadores
    T& operator*() {
        if (isNull()) throw std::runtime_error("Dereferencing null pointer");
        return *ptr;
    }

    MPointer<T>& operator=(const T& value) {
        *ptr = value;
        return *this;
    }

    MPointer<T>& operator=(const MPointer<T>& other) {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            MPointerGC::getInstance().addReference(ptr.get());
        }
        return *this;
    }

    // Verifica si el puntero interno es nullptr
    bool isNull() const {
        return !ptr;
    }
};

#endif
