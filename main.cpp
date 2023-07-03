
#include <assert.h>
#include <unistd.h>
#include "malloc_2.h"
#define MAX_ELEMENT_SIZE (128*1024)

int main() {
    // Initial state
//    verify_block_by_order(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

    // Allocate small block (order 0)
    void *ptr1 = smalloc(40);
    assert(ptr1 != nullptr);
//    verify_size(base);
//    verify_block_by_order(1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,31,0,0,0);

    // Allocate large block (order 10)
    void *ptr2 = smalloc(MAX_ELEMENT_SIZE+100);
    assert(ptr2 != nullptr);
//    verify_size_with_large_blocks(base, (128 * 1024+100 +_size_meta_data()));
//    verify_block_by_order(1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,31,0,1,MAX_ELEMENT_SIZE+100);

    // Allocate another small block
    void *ptr3 = smalloc(50);
    assert(ptr3 != nullptr);
//    verify_block_by_order(0,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,31,0,1,MAX_ELEMENT_SIZE+100);

    // Free the first small block
    sfree(ptr1);
//    verify_block_by_order(1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,31,0,1,MAX_ELEMENT_SIZE+100);


    // Allocate another small block
    void *ptr4 = smalloc(40);
    assert(ptr4 != nullptr);
//    verify_block_by_order(0,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,31,0,1,MAX_ELEMENT_SIZE+100);

    // Free all blocks
    sfree(ptr3);
    sfree(ptr4);
//    verify_block_by_order(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,1,MAX_ELEMENT_SIZE+100);
    sfree(ptr1); //free again
    sfree(ptr2);
//    verify_block_by_order(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0);
//    verify_size_with_large_blocks(base, 0);
//    verify_size(base);
}
