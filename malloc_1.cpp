#include <unistd.h>

void* smalloc(size_t size) {
    if (size == 0 || size > 100000000) {
        return nullptr;
    }
    void* sbrk_result = sbrk(size);
    if (sbrk_result == (void *)-1) {
        return nullptr;
    }
    return sbrk_result;
}