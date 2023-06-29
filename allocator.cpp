#include<iostream>
#include<list>
#include<algorithm>
#include<string>
using namespace std;

class block{
        int size,start_address;
        bool allocated;
        friend class memory_manager;

    public:
        block(int address,int n_size): start_address(address),size(n_size),allocated(false){}

        int getstart_address()const{
            return start_address;
        }
        int getsize()const{
            return size;
        }
        bool is_allocated()const{
            return allocated;
        }
};

class memory_manager{
    std::list<block>blocks;

    public:
        memory_manager(int blockSize){
            block init_block(0,blockSize);
            blocks.push_back(init_block);
        }

        void allocation(int size);
        void deallocation(int address);
        void defragmentation();
        void display();
};

void memory_manager::display(){
    cout<<"Memory allocation status: "<<endl;
    for(const auto& block : blocks){
        int address=block.start_address;
        int size=block.size;
        std::string status=(block.allocated? "Yes" : "No");
        cout<<"Address: "<<address << ",Size: "<<size;
        cout<<", Allocated: "<<status<<endl;
    }
}

void memory_manager::allocation(int size){
    for(auto b=blocks.begin();b!=blocks.end();++b){
        if((b->size>=size) && !(b->allocated)){
            if(b->size>size+sizeof(block)){
                int new_size=(b->size)-(size)-(sizeof(block));
                int new_start_address=(b->start_address)+size+sizeof(block);
                block new_block(new_start_address,new_size);
                blocks.insert(std::next(b),new_block);
                b->size=size;
            }
            b->allocated=true;
            cout<<"Memory block allocated address:"<<b->start_address<<endl;
            return;
        }
    }
}

void memory_manager::deallocation(int address){

    for(auto b=blocks.begin();b!=blocks.end();++b){
        if((b->start_address==address) && (b->allocated)){
            b->allocated=false;
            cout<<"Memory block is delocated from the address: "<<address<<endl;

            if(b!=blocks.begin()){
                auto prev_b=std::prev(b);
                if(!(prev_b->allocated)){
                    prev_b->size+=b->size+sizeof(block);
                    blocks.erase(b);
                    b=prev_b;
                }
            }
        
            /*if(std::next(b)!= blocks.end()){
                auto next_b=std::next(b);
                if(!(next_b->allocated)){
                    b->size+=next_b->size+sizeof(block);
                    blocks.erase(next_b);
                }
            }*/
            return;
        }
    }
    cout<<"Invalid memory address provided. Failure in deallocation"<<endl;
}

void memory_manager::defragmentation(){
    blocks.sort([](const block& a, const block& b) {
        return a.start_address < b.start_address;
    });

    auto b = blocks.begin();
    auto next = std::next(b);

    while (next != blocks.end()) {
        if (!b->is_allocated() && !next->is_allocated()) {
            b->size += next->size + sizeof(block);
            b = blocks.erase(next);
            next = std::next(b);
        } else {
            b++;
            next++;
        }
    }
}

int main(){
    int blocksize,address,size,choice;
    cout<<"Enter the block size: ";
    cin>>blocksize;

    memory_manager allocator(blocksize);

    while(1){
        cout<<"\n------ MENU ------"<<endl;
        cout<<"1. Allocate some memory"<<endl;
        cout<<"2. Deallocate some memory"<<endl;
        cout<<"3. Defragment the memory space"<<endl;
        cout<<"4. Display allocated memory status"<<endl;
        cout<<"5. Exit the allocator"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;

        switch(choice){
            case 1:
                cout<<"Enter the size of memory to be allocated: ";
                cin>>size;
                allocator.allocation(size);
                break;
            case 2:
                cout<<"Enter the address for deallocation: ";
                cin>>address;
                allocator.deallocation(address);
                break;
            case 3:
                allocator.defragmentation();
                break;
            case 4:
                allocator.display();
                break;
            case 5:
                cout<<"Exiting the allocator"<<endl;
                exit(0);
            default:
                cout<<"Invalid operation, choose the correct option"<<endl;

        }
    }

}


