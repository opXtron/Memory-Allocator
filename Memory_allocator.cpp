#include <iostream>
#include <vector>
#include<algorithm>


using namespace std;

class Memory_Allocator{
public:
    Memory_Allocator(int size):memorySize(size), allocator_on(true) {   //constructor function
        //memory.resize(memorySize,0);
        freeBlocks.push_back({0, memorySize});
    }

    // function to initialize the allocator
    void run(){
        while(allocator_on){
            displayMenu();      
            processMenuChoice();  
        }
    }

private:
    struct MemoryBlock {
        int start;      // start address of the block
        int size;          
    }; 
    
    bool allocator_on;    
    int memorySize;        // total memory size for allocating blocks
    
    //std::vector<char>memory;   
    std::vector<MemoryBlock>freeBlocks;  // vector to store free blocks 
    std::vector<MemoryBlock>allocatedBlocks; //vector to store allocated blocks

    void displayMenu() {
        cout<<"\n//-----Memory allocator:-----\\ \n";
        cout<<"a) Allocate a memory block\n";
        cout<<"b) Deallocate a memory block\n";
        cout<<"c) Display the allocation status\n";
        //cout<<"d) Perform defragmentation\n";
        cout<<"e) Exit the allocator\n";
        cout<<"Enter your choice(in terms of a,b,c,e): ";
    }

    void processMenuChoice() {
        char choice;
        cin>>choice;
        clearInputBuffer();  
        switch(choice){
            case 'a':
                allocateMemory();
                break;
            case 'b':
                deallocateMemory();
                break;
            case 'c':
                displayMemoryStatus();
                break;
            /*case 'd':
                defragmentMemory();
                break;*/
            case 'e':
                allocator_on = false;
                cout<<"\nExiting the program.\n";
                break;
            default:
                cout<<"Invalid choice. Please try again.\n";
                break;
        }
    }

    void allocateMemory() {
        defragmentMemory();
        cout<<"\nAllocation of memory:\n";
        int blockSize;              //block size
        cout<<"Enter block size: ";
        cin>>blockSize;           
        clearInputBuffer(); 
        if(blockSize!= 0){
            int blockIndex=findFreeBlock(blockSize);    //returns the index of free block
            if(blockIndex!=-1){
                allocateBlock(blockIndex, blockSize);     // allocate the block based on memory and index of free block
                cout<<"Memory block allocated successfully.\n";
            }
            else 
                cout<<"Cannot allocate memory block of size "<< blockSize <<". Not enough free memory.\n";  //free memory not found
        }
         
    }

    int findFreeBlock(int blockSize) {      //returns the index of the free block
        for (int i=0;i<freeBlocks.size();i++) {     
            if (freeBlocks[i].size>=blockSize) {
                return i;
            }
        }
        return -1;
    }

    void allocateBlock(int blockIndex, int blockSize) {
        MemoryBlock& block = freeBlocks[blockIndex];   //copy the free block details
        MemoryBlock allocatedBlock;    //creating a block to allocate
        allocatedBlock.start=block.start;   //address intialization
        allocatedBlock.size=blockSize;  //allocating required memory
        allocatedBlocks.push_back(allocatedBlock);  //pushing this block to the vector of allocated blocks

        if(block.size==blockSize) // free block size is equal to required memory
            freeBlocks.erase(freeBlocks.begin()+blockIndex);   
        else  // freeblock size > required block size
        {
            block.start+=blockSize;
            block.size-=blockSize;
        }
    }


    void deallocateMemory() {
        cout<<"\nDeallocate memory selected.\n";
        if(allocatedBlocks.empty()){          //if no memory is allocated
            cout<<"No memory blocks to deallocate.\n";
            return;
        }

        int blockIndex;
        cout<<"Enter index of the block to deallocate(zero based indexing): ";  //index of the block to deallocate
        cin>>blockIndex;
        clearInputBuffer();  // Clear input buffer

        if(blockIndex>= 0 && blockIndex<allocatedBlocks.size()) {   // valid block index condition
            deallocateBlock(blockIndex);        //deallocation function called
            cout<<"Memory block deallocated successfully.\n";
        } 
        else 
            cout<<"Invalid block index.\n";

    }

    void deallocateBlock(int blockIndex) {
        MemoryBlock& block=allocatedBlocks[blockIndex];   //copying the block to be deallocated
        freeBlocks.push_back(block);           
        allocatedBlocks.erase(allocatedBlocks.begin() + blockIndex);    //deallocate the block
        mergeFreeBlocks();  //merging the contiguous free blocks
    }

    void mergeFreeBlocks() {
        std::sort(freeBlocks.begin(), freeBlocks.end(), [](const MemoryBlock& a, const MemoryBlock& b) {
            return a.start<b.start;
        });         //sorting as blocks are pushed to the back of the vector

        for (int i=1;i<freeBlocks.size();i++) {
            MemoryBlock& currentBlock=freeBlocks[i];
            MemoryBlock& previousBlock=freeBlocks[i - 1];
            if (previousBlock.start+previousBlock.size==currentBlock.start) {
                previousBlock.size+=currentBlock.size;
                freeBlocks.erase(freeBlocks.begin()+i);
                i--;
            }
        }
    }

    void displayMemoryStatus() {
        cout<<"\nMemory allocation status selected.\n";
        if(memorySize==0)  //no allocation
            cout<<"No memory has been allocated.\n";
        else
        {
            cout<<"Memory Size: "<<memorySize<<"\n";
            cout<<"Number of Allocated Blocks: "<<allocatedBlocks.size()<<"\n";
            for (int i=0;i<allocatedBlocks.size();i++)  //print allocated blocks
            {
                const MemoryBlock& block=allocatedBlocks[i];
                cout<<"Block "<<(i+1)<< " Size: "<<block.size<<", Start Address: "<<block.start<<", Index: "<< i <<"\n";
            }


            cout<<"Number of Free Blocks/holes: "<<freeBlocks.size()<<"\n"; 
            if(freeBlocks.size()!=0){
                for(int i=0;i<freeBlocks.size();i++){       
                const MemoryBlock& block=freeBlocks[i];
                cout<<"Free Block "<<(i + 1)<<" Size: "<<block.size<<", Start Address: "<<block.start<<"\n";
            }
            } 
            
        }
    }

    void defragmentMemory(){
    // Sorting allocated blocks by the means of their start address
    std::sort(allocatedBlocks.begin(),allocatedBlocks.end(),[](const MemoryBlock& a, const MemoryBlock& b) {
        return a.start < b.start;
    });

    //Adjusting/PLACING the allocated blocks based on their start address
    int currentAddress=0;
    for(auto& block:allocatedBlocks){
        block.start=currentAddress;
        currentAddress+=block.size;
    }

    // updating the freeblock status
    freeBlocks.clear();
    MemoryBlock freeBlock;
    freeBlock.start=currentAddress;
    freeBlock.size=memorySize-currentAddress;
    freeBlocks.push_back(freeBlock);
    }

    // function to clear input buffer
    void clearInputBuffer() {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

int main() {
    int memorySize;
    cout<<"Enter the total memory size: ";
    cin>>memorySize;
    fflush(stdin);  // Clear input buffer

    Memory_Allocator Memory_Allocator(memorySize);  //creating an object and specifying the total memory size
    Memory_Allocator.run();     //calling or running the allocator.

    return 0;
}
