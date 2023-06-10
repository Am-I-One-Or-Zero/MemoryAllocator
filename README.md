# Efficient memory allocator

## Brief description
*Pool memory allocator* is essential tool for video games and other programs
where it is neccessary to *allocate and free* a lot of objects
that do not have very big size.
Memory allocator reserves a block of memory at construction.
When there will be no available memory then it will allocate
in *grow coefficient* times more memory than in previous time and so on.

## Features
* **Extremely fast** when allocating and freeing a lot of objects
* Prevents **memory fragmentation**
* Easily extensible and **flexible**
* Allocation size can **grow** just like std::vector do
* **Single-header** library
* Very **lightweight** and small (only 130 lines of code)
* Use of **C++ 11** standard

## Recommended usage
The best way to use it all is to reserve **much** memory at construction
and then manage it as needed.
Grow coefficient should be in range [1.0; 2.0]

## Tutorial 
``` cpp
#define POOL_ALLOCATOR_IMPL  /* Include definitions also */
#include <PoolAllocator.hpp> 


struct Object 
{
    double number;
    char name[256];
};

int main()
{
    PoolAllocator allocator(sizeof(Object), 1000, 1.5f);
    /* Constructor parameters:
        - Size of object
        - Number of objects to allocate at construction (1 by default)
        - Grow coefficient (1.0f by default) */

    Object* pointer = (Object*)allocator.allocate();
    /* Returns pointer to allocated object.
     * You can do it as much as you want. */

    allocator.deallocate(pointer);
    /* Takes pointer to allocated object */

    allocator.clear();
    /* Allocator will free all the allocated memory automatically.
     * But it can be done manually using 'clear' method */
}
```

## License
Do **everything** you want. See LICENSE.txt

