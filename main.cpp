
#include <assert.h>
#include <unistd.h>
#include <valarray>
#include <vector>
#include "malloc_2.h"
#define MAX_ELEMENT_SIZE (128*1024)
#define verify_blocks(allocated_blocks, allocated_bytes, free_blocks, free_bytes)                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        assert(_num_allocated_blocks() == allocated_blocks);                                                          \
        assert(_num_allocated_bytes() == (allocated_bytes));                                              \
        assert(_num_free_blocks() == free_blocks);                                                                    \
        assert(_num_free_bytes() == (free_bytes));                                                        \
        assert(_num_meta_data_bytes() == (_size_meta_data() * allocated_blocks));                         \
    } while (0)

#define verify_size(base)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        void *after = sbrk(0);                                                                                         \
        assert(_num_allocated_bytes() + aligned_size(_size_meta_data() * _num_allocated_blocks()) ==                  \
                (size_t)after - (size_t)base);                                                                         \
    } while (0)

#define verify_size_with_large_blocks(base, diff)                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        void *after = sbrk(0);                                                                                         \
        assert(diff == (size_t)after - (size_t)base);                                                                 \
    } while (0)
void verify_block_by_order(int order0free, int order0used, int order1free, int order1used, \
                                int order2free, int order2used,\
                                int order3free, int order3used, \
                                int order4free, int order4used, \
                                int order5free, int order5used, \
                                int order6free, int order6used, \
                                int order7free, int order7used, \
                                int order8free,int  order8used, \
                                int order9free,int  order9used, \
                                int order10free,int  order10used,
                           int big_blocks_count, long big_blocks_size  )\
                                                                                                                     \
    {                                                                                                                  \
        unsigned int __total_blocks = order0free + order0used+ order1free + order1used+ order2free + order2used+ order3free + order3used+ order4free + order4used+ order5free + order5used+ order6free + order6used+ order7free + order7used+ order8free + order8used+ order9free + order9used+ order10free + order10used + big_blocks_count       ;        \
        unsigned int __total_free_blocks = order0free+ order1free+ order2free+ order3free+ order4free+ order5free+ order6free+ order7free+ order8free+ order9free+ order10free ;                     \
        unsigned int __total_free_bytes_with_meta  = order0free*128*pow(2,0) +  order1free*128*pow(2,1) +  order2free*128*pow(2,2) +  order3free*128*pow(2,3) +  order4free*128*pow(2,4) +  order5free*128*pow(2,5) +  order6free*128*pow(2,6) +  order7free*128*pow(2,7) +  order8free*128*pow(2,8) +  order9free*128*pow(2,9)+  order10free*128*pow(2,10) ;                                                                     \
        unsigned int testing_allocated_bytes;
    if (__total_blocks==0) testing_allocated_bytes = 0;
    else testing_allocated_bytes = big_blocks_size+32 * MAX_ELEMENT_SIZE - (__total_blocks-big_blocks_count)*(_size_meta_data());
    verify_blocks(__total_blocks, testing_allocated_bytes, __total_free_blocks,__total_free_bytes_with_meta - __total_free_blocks*(_size_meta_data()));\
    }


int main() {
    std::vector<void*> allocations;

    // Allocate 64 blocks of size 128 * 2^9 - 64
    for (int i = 0; i < 64; i++)
    {
        void* ptr = smalloc(128 * std::pow(2, 9) - 64);
        assert(ptr != nullptr);
        allocations.push_back(ptr);
//        printf("%d\n",i);
//        fflush(stdout);
        verify_block_by_order(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, allocations.size()%2, allocations.size(), 32-(int)(i/2)-1, 0, 0, 0);
    }

    assert(smalloc(40) == NULL);
    // Free the allocated blocks
    while (!allocations.empty())
    {
        void* ptr = allocations.back();
        allocations.pop_back();
        sfree(ptr);
        verify_block_by_order(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, allocations.size() % 2, allocations.size(), 32 - (int)(allocations.size() / 2) -(allocations.size() % 2), 0, 0, 0);
    }

    // Verify that all blocks are merged into a single large block
    verify_block_by_order(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0);


    for (int i = 0; i < 64; i++)
    {
        void* ptr = smalloc(128 * std::pow(2, 9) - 64);
        assert(ptr != nullptr);
        allocations.push_back(ptr);
        verify_block_by_order(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, allocations.size()%2, allocations.size(), 32-(int)(i/2)-1, 0, 0, 0);
    }
    assert(smalloc(40) == NULL);
    // Free the allocated blocks
    while (!allocations.empty())
    {
        void* ptr = allocations.front();
        allocations.erase(allocations.begin());
        sfree(ptr);
        verify_block_by_order(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, allocations.size() % 2, allocations.size(), 32 - (int)(allocations.size() / 2) -(allocations.size() % 2), 0, 0, 0);
    }
    verify_block_by_order(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0);
        return 0;
}
