#include<iostream>
#include<vector>
using namespace std;

class MemoryAllocator{
    
    class block{
        size_t size;
        bool allocated;
        friend class MemoryAllocator;
    };
    std::vector<block> memoryBlocks;

    public:
        void* allocate(size_t size);
        void deallocate(void* ptr);
        void defragment();
        void displayStatus() const; // const member function
};


