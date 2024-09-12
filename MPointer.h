#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <queue>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

class MPointerGC {
private:
    vector<void*> memoryPool;
    unordered_map<int, int> referenceCounts;
    int currentIndex;
    atomic<bool> running;            // Para controlar el hilo
    int interval;                    // Intervalo de tiempo en segundos
    thread gcThread;                 // Hilo para el recolector de basura

    MPointerGC() : currentIndex(0), running(false), interval(5) {}

    void gcRunner() {
        while (running) {
            this_thread::sleep_for(chrono::seconds(interval));
            cout << "[MPointerGC] Ejecutando ciclo de recolección..." << endl;
        }
    }

public:
    static MPointerGC& getInstance() {
        static MPointerGC instance;
        return instance;
    }

    // Iniciar el hilo de GC con un intervalo de n segundos
    void startGC(int n) {
        if (!running) {
            interval = n;
            running = true;
            gcThread = thread(&MPointerGC::gcRunner, this);
            cout << "[MPointerGC] Gestor de memoria iniciado con intervalo de " << n << " segundos." << endl;
        }
    }

    // Verificar si el GC está corriendo
    bool isRunning() const {
        return running;
    }

    void stopGC() {
        if (running) {
            running = false;
            if (gcThread.joinable()) {
                gcThread.join();
            }
            cout << "[MPointerGC] Gestor de memoria detenido." << endl;
        }
    }

    ~MPointerGC() {
        stopGC();
    }

    int allocate(size_t size) {
        void* newMem = malloc(size);
        if (!newMem) throw bad_alloc();

        int index = currentIndex++;
        memoryPool.push_back(newMem);
        referenceCounts[index] = 1;

        return index;
    }

    void deallocate(int index) {
        if (referenceCounts[index] == 0 && memoryPool[index] != nullptr) {
            free(memoryPool[index]);
            memoryPool[index] = nullptr;
        } else {
            return;
        }
    }

    void addReference(int index) {
        referenceCounts[index]++;
    }

    void removeReference(int index) {
        if (index == -1) return;

        if (referenceCounts.find(index) != referenceCounts.end() && referenceCounts[index] > 0) {
            referenceCounts[index]--;
            if (referenceCounts[index] == 0) {
                deallocate(index);
            }
        } else {
            throw runtime_error("Error: Invalid attempt to reduce reference count at index " + to_string(index));
        }
    }
    
    void* getMemory(int index) {
        if (index >= memoryPool.size() || memoryPool[index] == nullptr) {
            throw runtime_error("[MPointerGC] Invalid memory access at index " + to_string(index));
        }
        return memoryPool[index];
    }
};

template<typename T>
class MPointer {
private:
    int memIndex;

    MPointer(int index) : memIndex(index) {}

public:
    MPointer() : memIndex(-1) {}

    // Crear un nuevo MPointer con memoria asignada
    static MPointer<T> New() {
        if (!MPointerGC::getInstance().isRunning()) {
            throw runtime_error("MPointerGC must be started before using MPointer.");
        }
        int index = MPointerGC::getInstance().allocate(sizeof(T));
        return MPointer<T>(index);
    }

    ~MPointer() {}

    T& operator*() {
        if (memIndex == -1) throw runtime_error("Dereferencing null pointer");
        return *(T*)(MPointerGC::getInstance().getMemory(memIndex));
    }

    MPointer<T>& operator=(const MPointer<T>& other) {
        if (this != &other) {
            if (!other.isNull()) {
                memIndex = other.memIndex;
            } else {
                memIndex = -1;
            }
        }
        return *this;
    }

    bool isNull() const {
        return memIndex == -1;
    }

    int getIndex() const {
        return memIndex;
    }
};
#endif
