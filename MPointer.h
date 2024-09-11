#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <queue>

using namespace std;

class MPointerGC {
private:
    vector<void*> memoryPool;
    unordered_map<int, int> referenceCounts;
    int currentIndex;

    MPointerGC() : currentIndex(0) {}

public:
    static MPointerGC& getInstance() {
        static MPointerGC instance;
        return instance;
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

    static MPointer<T> New() {
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
            memIndex = other.memIndex;
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
