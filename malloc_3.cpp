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
    unsigned int indexInArray;
    bool isMmapped;
};

unsigned int getMatchingIndex(size_t size);

unsigned int getMatchingFirstIndex(size_t size);

bool isAdjacent(const MallocMetadata *block1, const MallocMetadata *block2);

bool isMergedBlockSizeOk(const MallocMetadata *block);

size_t getAllocationSize(size_t size);

void setMmappedBlockData(size_t allocation_size, const void *mmap_res);

void *getMmapedBlock(size_t size);

void setAllocatedBlocksBytes(size_t size);

unsigned int handleBlockSplit(unsigned int indexInArray, MallocMetadata *availableBlock);

MallocMetadata *insertFirstBlockToList(MallocMetadata *block1, const MallocMetadata *block2);

MallocMetadata * blockMergeToMaxOrSize(MallocMetadata *pMetaData, bool mergeToMax, unsigned int mergedIndex = 0);

bool canBeReallocedAtSize(MallocMetadata *pMetadata, unsigned int requiredIndexForSize);

bool canBeSpeculatedMergedOnce(MallocMetadata *pMetadata, unsigned int index);

void cookieAuthenticator(MallocMetadata *pMetadata);

MallocMetadata* freeListsArray[MAX_ORDER + 1] = {nullptr};
bool didWeAllocate = false;
size_t num_allocated_blocks = 0;
size_t num_allocated_bytes = 0;
int globalCookie = -1;

size_t _size_meta_data() {
    return sizeof(MallocMetadata);
}

void _num_free_blocks_unique_action(MallocMetadata* temp, size_t& num_free_blocks) {
    num_free_blocks++;
}

void _num_free_bytes_unique_action(MallocMetadata* temp, size_t& num_free_bytes) {
    num_free_bytes+=temp->size;
}

void _num_allocated_blocks_unique_action(MallocMetadata* temp, size_t& num_allocated_blocks) {
    num_allocated_blocks++;
}

void _num_allocated_bytes_unique_action(MallocMetadata* temp, size_t& num_allocated_bytes) {
    num_allocated_bytes+=temp->size;
}

//void _num_meta_data_bytes_unique_action(MallocMetadata* temp, size_t& num_meta_data_bytes) {
//    num_meta_data_bytes += _size_meta_data();
//}

size_t _generic_function_for_all_methods(void (*unique_action)(MallocMetadata*, size_t&)) {
    size_t num = 0;
    for (unsigned int i = 0; i <= MAX_ORDER; i++) {
        MallocMetadata* temp = freeListsArray[i];
        while (temp) {
            unique_action(temp, num);
            temp = temp->freeListNext;
        }
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
    return num_allocated_blocks;
}

size_t _num_allocated_bytes () {
    return num_allocated_bytes;
}

size_t _num_meta_data_bytes() {
//    return _generic_function_for_all_methods(_num_meta_data_bytes_unique_action);
    return 0;
}

bool initialAllocation() {
    globalCookie = rand();
    if (sbrk(0) && (unsigned long)sbrk(0) % (INITIAL_BLOCKS_AMOUNT * MAX_BLOCK_SIZE) > 0) {
        sbrk((INITIAL_BLOCKS_AMOUNT * MAX_BLOCK_SIZE) - (unsigned long)sbrk(0) % (INITIAL_BLOCKS_AMOUNT * MAX_BLOCK_SIZE));
    }
    for (unsigned int i = 0; i < INITIAL_BLOCKS_AMOUNT; i++) {
        void* sbrk_result = sbrk(MAX_BLOCK_SIZE);
        if (!sbrk_result) {
            return false;
        }
        MallocMetadata* temp = freeListsArray[MAX_ORDER];
        MallocMetadata* new_node = (MallocMetadata*)sbrk_result;
        new_node->size = MAX_BLOCK_SIZE;
        new_node->cookie = globalCookie;
        new_node->freeListNext = nullptr;
        new_node->isMmapped = false;
        new_node->indexInArray = MAX_ORDER;
        while (temp && temp->freeListNext) {
            temp = temp->freeListNext;
        }
        if (!temp) {
            freeListsArray[MAX_ORDER] = new_node;
            new_node->freeListPrev = nullptr;
        }
        else {
            temp->freeListNext = new_node;
            new_node->freeListPrev = temp;
        }
    }
    return true;
}

MallocMetadata *insertToFreeListAt(unsigned int index, void *beginning_address) {
    MallocMetadata* temp = freeListsArray[index];
    auto* new_node = (MallocMetadata*)beginning_address;
    new_node->size = (128 << index);
    new_node->cookie = globalCookie;
    new_node->indexInArray = index;
    new_node->isMmapped = false;
    if (!temp) {
        new_node->freeListPrev=nullptr;
        new_node->freeListNext=nullptr;
        freeListsArray[index] = new_node;
        return new_node;
    }
    cookieAuthenticator(temp);
    MallocMetadata* prev = temp->freeListPrev;
    while(temp->freeListNext && (unsigned long)beginning_address > (unsigned long)temp) {
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

MallocMetadata* findBuddyAddress(MallocMetadata* block) {
    auto blockAddress = (unsigned long)block;
    unsigned long buddyAddress = blockAddress^(block->size);
    cookieAuthenticator((MallocMetadata*)buddyAddress);
    return (MallocMetadata*)buddyAddress;
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
    while (power_of_2 < MAX_ORDER && ((unsigned long)128 << power_of_2) < (size + _size_meta_data())) {
        power_of_2++;
    }
    return power_of_2;
}


void removeFromFreeList(MallocMetadata* block) {
    if (!block) {
        return;
    }
    if (!block->freeListPrev) {
        freeListsArray[block->indexInArray] = block->freeListNext;
    }
    else {
        block->freeListPrev->freeListNext = block->freeListNext;
        block->freeListPrev = nullptr;
    }
    if (block->freeListNext) {
        block->freeListNext->freeListPrev = block->freeListPrev;
        block->freeListNext = nullptr;
    }
}

void* allocateFromFreeList(size_t size) {
    unsigned int indexInArray = getMatchingIndex(size);
    if (indexInArray > MAX_ORDER || !freeListsArray[indexInArray]) {
        return nullptr;
    }
    MallocMetadata* availableBlock = freeListsArray[indexInArray];
    if(!availableBlock) {
        return nullptr;
    }
    cookieAuthenticator(availableBlock);
    while ((unsigned long)128 << (indexInArray - 1) > size + _size_meta_data() && indexInArray > 0) {
        indexInArray = handleBlockSplit(indexInArray, availableBlock);
    }
    return (void*)((unsigned long)availableBlock + _size_meta_data());
}

unsigned int handleBlockSplit(unsigned int indexInArray, MallocMetadata *availableBlock) {
    removeFromFreeList(availableBlock);
    availableBlock->size /= 2;
    insertToFreeListAt(indexInArray - 1, (void *) availableBlock);
    insertToFreeListAt(indexInArray - 1, (void *) ((unsigned long)availableBlock + availableBlock->size));
    indexInArray--;
    return indexInArray;
}

void* smalloc(size_t size){
    if (size == 0 || size > 100000000) {
        return nullptr;
    }
    if (!didWeAllocate) {
        didWeAllocate = true;
        if (!initialAllocation()) {
            return nullptr;
        }
    }
    if (size > MAX_BLOCK_SIZE - _size_meta_data()) {
        return getMmapedBlock(size);
    }
    void* allocated_address = allocateFromFreeList(size);
    removeFromFreeList((MallocMetadata*)allocated_address);
    setAllocatedBlocksBytes(size);
    return allocated_address;
}

void setAllocatedBlocksBytes(size_t size) {
    num_allocated_blocks++;
    num_allocated_bytes += size;
}

void *getMmapedBlock(size_t size) {
    size_t allocation_size = getAllocationSize(size);
    void* mmap_res = mmap(nullptr,allocation_size,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,-1,0);
    if (mmap_res == (void *) -1) {
        return nullptr;
    }
    setMmappedBlockData(allocation_size, mmap_res);
    setAllocatedBlocksBytes(size);
    return (void*)((unsigned long)mmap_res+_size_meta_data());
}

void setMmappedBlockData(size_t allocation_size, const void *mmap_res) {
    auto* mmappedMetaData = (MallocMetadata*)mmap_res;
    mmappedMetaData->size = allocation_size-_size_meta_data();
    mmappedMetaData->isMmapped = true;
    mmappedMetaData->cookie = globalCookie;
    mmappedMetaData->freeListNext = nullptr;
    mmappedMetaData->freeListPrev = nullptr;
}

size_t getAllocationSize(size_t size) {
    size_t allocation_size = (size + _size_meta_data()) / 4096;
    allocation_size*=4096;
    if((size+_size_meta_data()) % 4096 > 0) {
        allocation_size+= 4096;
    }
    return allocation_size;
}

void* scalloc(size_t num, size_t size) {
    void* tozeroall= smalloc(num*size);
    if (tozeroall) {
        std::memset(tozeroall,0,num*size);
    }
    return tozeroall;
}

MallocMetadata* mergeBlocks (MallocMetadata* block1, MallocMetadata* block2) {
    removeFromFreeList(block1);
    removeFromFreeList(block2);
    if ((unsigned long)block2 > (unsigned long)block1) {
        return insertFirstBlockToList(block1, block2);
    }
    return insertFirstBlockToList(block2, block1);
}

MallocMetadata *insertFirstBlockToList(MallocMetadata *block1, const MallocMetadata *block2) {
    block1->size += block2->size + _size_meta_data();
    return insertToFreeListAt(block1->indexInArray + 1, block1);
}

bool isAdjacent(const MallocMetadata *block1, const MallocMetadata *block2) {
    if (!block1 || !block2) {
        return false;
    }
    if ((unsigned long)block1 > (unsigned long)block2) {
        return (unsigned long)block2 + (128 << block2->indexInArray) == (unsigned long)block1
        && block2->freeListNext == block1 && block1->freeListPrev == block2;
    }
    return (unsigned long)block1 + (128 << block1->indexInArray) == (unsigned long)block2
           && block1->freeListNext == block2 && block2->freeListPrev == block1;
}

bool canBeMerged(MallocMetadata* block) {
    MallocMetadata* buddy = findBuddyAddress(block);
    if (buddy && buddy->indexInArray == block->indexInArray && block->indexInArray < MAX_ORDER && isAdjacent(block, buddy)) {
        return true;
    }
    return false;
}

void sfree(void* p){
    auto* pMetaData = (MallocMetadata*)((unsigned long)p-_size_meta_data());
    cookieAuthenticator(pMetaData);
    if (pMetaData->isMmapped) {
        munmap(pMetaData, pMetaData->size+_size_meta_data());
        return;
    }
    pMetaData = insertToFreeListAt(getMatchingIndex(pMetaData->size), p);
    blockMergeToMaxOrSize(pMetaData, true);
}

void cookieAuthenticator(MallocMetadata *pMetadata) {
    if (pMetadata->cookie != globalCookie) {
        exit(0xdeadbeef);
    }
}

MallocMetadata * blockMergeToMaxOrSize(MallocMetadata *pMetaData, bool mergeToMax, unsigned int mergedIndex) {
    while(canBeMerged(pMetaData) && (mergeToMax || pMetaData->indexInArray < mergedIndex)) {
        MallocMetadata* buddy = findBuddyAddress(pMetaData);
        if(buddy) {
            pMetaData = mergeBlocks(pMetaData, buddy);
        }
        else {
            break;
        }
    }
    return pMetaData;
}

void* srealloc(void* oldp, size_t size){
    if (size == 0 || size > 100000000) {
        return nullptr;
    }
    if(!oldp)
    {
        return smalloc(size);
    }
    MallocMetadata* oldp_meta_data = ((MallocMetadata*)((unsigned long)oldp - _size_meta_data()));
    cookieAuthenticator(oldp_meta_data);
    if(size <= oldp_meta_data->size)
    {
        return oldp;
    }
    if(size <= (unsigned long)128 << oldp_meta_data->indexInArray)
    {
        oldp_meta_data->size = size;
        return oldp;
    }
    unsigned int requiredIndexForSize = getMatchingFirstIndex(size+_size_meta_data());
    if (canBeReallocedAtSize(oldp_meta_data, requiredIndexForSize)) {
        MallocMetadata* newp_meta_data = blockMergeToMaxOrSize(oldp_meta_data, false, requiredIndexForSize);
        cookieAuthenticator(newp_meta_data);
        removeFromFreeList(newp_meta_data);
        return (void*)((unsigned long)newp_meta_data + _size_meta_data());
    }
    void* reallocated_space=smalloc(size);
    if(reallocated_space)
    {
        memmove(reallocated_space,oldp,size);
        sfree(oldp);
        return reallocated_space;
    }
    return nullptr;
}

bool canBeReallocedAtSize(MallocMetadata *pMetadata, unsigned int requiredIndexForSize) {
    unsigned int speculativeIndex = pMetadata->indexInArray;
    while (canBeSpeculatedMergedOnce(pMetadata, speculativeIndex) && speculativeIndex < requiredIndexForSize) {
        MallocMetadata* buddy = findBuddyAddress(pMetadata);
        if ((unsigned long)buddy < (unsigned long)pMetadata) {
            pMetadata = buddy;
        }
        speculativeIndex++;
    }
    return requiredIndexForSize == speculativeIndex;
}

bool canBeSpeculatedMergedOnce(MallocMetadata *pMetadata, unsigned int index) {
    MallocMetadata* buddy = findBuddyAddress(pMetadata);
    if (buddy && buddy->indexInArray == index && index < MAX_ORDER && isAdjacent(pMetadata, buddy)) {
        return true;
    }
    return false;
}