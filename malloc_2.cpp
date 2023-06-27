#include <unistd.h>


class MallocMetadata {
public:
    size_t size;
    bool is_free;
    MallocMetadata* next;
    MallocMetadata* prev;
};


MallocMetadata* mallocMetaDataListHead= NULL;
MallocMetadata* mallocMetaDataListTail = NULL;

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

MallocMetadata* smalloc_reUse(size_t size) {
    MallocMetadata* temp = mallocMetaDataListHead;
    while (temp) {
        if(temp->is_free && temp->size>=size) {
            temp->is_free = false;
            break;
        }
        temp = temp->next;
    }
    return temp;
}

void* smalloc_newUse(size_t size)
{
    void* sbrk_result = sbrk(size+_size_meta_data());
    if (sbrk_result == (void *)-1) {
        return NULL;
    }
    MallocMetadata* new_node = (MallocMetadata*)sbrk_result;
    new_node->is_free = false;
    new_node->next = NULL;
    new_node->prev = mallocMetaDataListTail;
    new_node->size = size;
    if (mallocMetaDataListTail) {
        mallocMetaDataListTail->next = new_node;
        mallocMetaDataListTail = new_node;
        return sbrk_result;
    }
    mallocMetaDataListHead = new_node;
    mallocMetaDataListTail = new_node;
    return sbrk_result;
}

void* smalloc(size_t size){
    if (size == 0 || size > 100000000) {
        return NULL;
    }
    void* to_reuse= smalloc_reUse(size);
    if(to_reuse){
        return (char*)to_reuse + _size_meta_data();
    }
    void* new_use = smalloc_newUse(size);
    if (new_use) {
        return (char*)new_use + _size_meta_data();
    }
    return NULL;
}