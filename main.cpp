
#include <assert.h>
#include <unistd.h>
#include "malloc_2.h"

int main() {

    void *base = sbrk(0);
    int *a = (int *)srealloc(nullptr, 100 * sizeof(int));
    assert(a != nullptr);

    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
    }
    int *b = (int *)srealloc(a, 10 * sizeof(int));
    assert(b != nullptr);
   assert(b == a);
    for (int i = 0; i < 10; i++)
    {
        assert(b[i] == i);
    }


    sfree(b);
}
