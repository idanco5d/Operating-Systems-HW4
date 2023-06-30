#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/mman.h>

#define MAX_ORDER 10
#define INITIAL_BLOCKS_AMOUNT 32
#define MAX_BLOCK_SIZE 1024*128

class MallocMetadata {
public:
    int cookie;
    size_t size;
    MallocMetadata* freeListNext;
    MallocMetadata* freeListPrev;
};

unsigned int getMatchingIndex(size_t size);

unsigned int getMatchingFirstIndex(size_t size);

MallocMetadata* freeListsArray[MAX_ORDER + 1] = {NULL};
bool didWeAllocate = false;

size_t _size_meta_data() {
    return sizeof(MallocMetadata);
}

void _num_free_blocks_unique_action(MallocMetadata* temp, size_t& num_free_blocks) {
    if (temp->is_free) {
        num_free_blocks++;
    }
}

void _num_free_bytes_unique_action(MallocMetadata* temp, size_t& num_free_bytes) {
    if (temp->is_free) {
        num_free_bytes+=temp->size;
    }
}

void _num_allocated_blocks_unique_action(MallocMetadata* temp, size_t& num_allocated_blocks) {
    num_allocated_blocks++;
}

void _num_allocated_bytes_unique_action(MallocMetadata* temp, size_t& num_allocated_bytes) {
    num_allocated_bytes+=temp->size;
}

void _num_meta_data_bytes_unique_action(MallocMetadata* temp, size_t& num_meta_data_bytes) {
    num_meta_data_bytes += _size_meta_data();
}

size_t _generic_function_for_all_methods(void (*unique_action)(MallocMetadata*, size_t&)) {
    MallocMetadata* temp = mallocMetaDataListHead;
    size_t num = 0;
    while (temp) {
        unique_action(temp, num);
        temp = temp->next;
    }
    return num;
}

size_t _num_free_blocks() {
    return _generic_function_for_all_methods(_num_free_blocks_unique_action);
}

size_t _num_free_bytes() {
    return _generic_function_for_all_methods(_num_free_bytes_unique_action);
}

size_t _num_allocated_blocks () {
    return _generic_function_for_all_methods(_num_allocated_blocks_unique_action);
}

size_t _num_allocated_bytes () {
    return _generic_function_for_all_methods(_num_allocated_bytes_unique_action);
}

size_t _num_meta_data_bytes() {
    return _generic_function_for_all_methods(_num_meta_data_bytes_unique_action);
}

bool initialAllocation() {
    int cookiesForAll = rand();
    if (sbrk(0) && (unsigned long)sbrk(0) % (INITIAL_BLOCKS_AMOUNT * MAX_BLOCK_SIZE) > 0) {
        sbrk((unsigned long)sbrk(0) % (INITIAL_BLOCKS_AMOUNT * MAX_BLOCK_SIZE));
    }
    for (unsigned int i = 0; i < INITIAL_BLOCKS_AMOUNT; i++) {
        void* sbrk_result = sbrk(MAX_BLOCK_SIZE);
        if (!sbrk_result) {
            return false;
        }
        MallocMetadata* temp = freeListsArray[MAX_ORDER];
        MallocMetadata* new_node = (MallocMetadata*)sbrk_result;
        new_node->size = MAX_BLOCK_SIZE - _size_meta_data();
        new_node->cookie = cookiesForAll;
        new_node->freeListNext = NULL;
        while (temp && temp->freeListNext) {
            temp = temp->freeListNext;
        }
        if (!temp) {
            freeListsArray[MAX_ORDER] = new_node;
            new_node->freeListPrev = NULL;
        }
        else {
            temp->freeListNext = new_node;
            new_node->freeListPrev = temp;
        }
    }
    return true;
}

MallocMetadata* insertToFreeListAt(unsigned int index, void *beginning_address, int cookie) {
    MallocMetadata* temp = freeListsArray[index];
    MallocMetadata* new_node = (MallocMetadata*)beginning_address;
    new_node->size = (128 << index) - _size_meta_data();
    new_node->cookie = cookie;
    if (!temp) {
        new_node->freeListPrev=NULL;
        new_node->freeListNext=NULL;
        freeListsArray[index] = new_node;
        return;
    }
    MallocMetadata* prev = temp->freeListPrev;
    while(temp && (unsigned long)beginning_address > (unsigned long)temp) {
        prev = temp;
        temp = temp->freeListNext;
    }
    if (prev) {
        prev->freeListNext = new_node;
    }
    else {
        freeListsArray[index] = new_node;
    }
    temp->freeListPrev = new_node;
    new_node->freeListPrev = prev;
    new_node->freeListNext = temp;
    return new_node;
}

void* allocateFromFreeList(size_t size) {
    unsigned int power_of_2 = getMatchingIndex(size);
    if (power_of_2 > MAX_ORDER || !freeListsArray[power_of_2]) {
        return NULL;
    }
    MallocMetadata* availableBlock = freeListsArray[power_of_2];
    freeListsArray[power_of_2]->freeListNext->freeListPrev = NULL;
    freeListsArray[power_of_2] = freeListsArray[power_of_2]->freeListNext;
    while (128 << (power_of_2-1) > size+_size_meta_data()) {
        availableBlock->size -= (128 << (power_of_2-1));
        insertToFreeListAt(power_of_2 - 1, (void *) ((unsigned long) availableBlock + (128 << (power_of_2 - 1))), availableBlock->cookie);
        power_of_2--;
    }
    return (void*)((unsigned long)availableBlock + _size_meta_data());
}

void* smalloc(size_t size){
    if (size == 0 || size > 100000000) {
        return NULL;
    }
    if (!didWeAllocate) {
        didWeAllocate = true;
        if (!initialAllocation()) {
            return NULL;
        }
    }
    if (size > MAX_BLOCK_SIZE - _size_meta_data()) {
        size_t allocation_size = size/4096;
        allocation_size*=4096;
        if(size % 4096 > 0) {
            allocation_size+= 4096;
        }
        void* mmap_res = mmap(NULL,allocation_size,PROT_READ | PROT_WRITE, MAP_ANONYMOUS,-1,0);
        if (mmap_res == (void *) -1) {
            return NULL;
        }
        return mmap_res;
    }
    void* allocated_address = allocateFromFreeList(size);
    return allocated_address;
}

unsigned int getMatchingIndex(size_t size) {
    unsigned int power_of_2 = getMatchingFirstIndex(size);
    while (!freeListsArray[power_of_2]) {
        power_of_2++;
    }
    return power_of_2;
}

unsigned int getMatchingFirstIndex(size_t size) {
    unsigned int power_of_2 = 0;
    while (power_of_2 < MAX_ORDER && (128 << power_of_2) < (size + _size_meta_data())) {
        power_of_2++;
    }
    return power_of_2;
}


void* scalloc(size_t num, size_t size) {
    void* tozeroall= smalloc(num*size);
    if (tozeroall) {
        std::memset(tozeroall,0,num*size);
    }
    return tozeroall;
}

void sfree(void* p){
    MallocMetadata* pMetaData = (MallocMetadata*)p;
    unsigned int indexInArray = getMatchingIndex(pMetaData->size);
    pMetaData = insertToFreeListAt()
}

void* srealloc(void* oldp, size_t size){
    if (size == 0 || size > 100000000) {
        return NULL;
    }

    if(!oldp)
    {
        return smalloc(size);
    }

    if(size <= ((MallocMetadata*)((unsigned long)oldp - _size_meta_data()))->size)
    {
        return oldp;
    }

    void* reallocated_space=smalloc(size);
    if(reallocated_space)
    {
        memmove(reallocated_space,oldp,size);
        sfree(oldp);
        return reallocated_space;
    }
    return NULL;
}



