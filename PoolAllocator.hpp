#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP

/* This is the header-only library.
 * To include implementation uncomment next line (make it in .cpp file)
 * #define POOL_ALLOCATOR_IMPL */

#include <cstddef>


class PoolAllocator
{
    private:
            struct Node
            {
                Node*               next;
            };


    public:
            explicit                PoolAllocator(
                                        size_t objectSize,
                                        size_t initialAllocationSize=1,
                                        float  growCoefficient=1.f) noexcept;
            virtual                 ~PoolAllocator();
                                    PoolAllocator(
                                              const PoolAllocator&) = delete;
            PoolAllocator&          operator=(const PoolAllocator&) = delete;

            inline void*            allocate() noexcept;
            inline void             deallocate(void*) noexcept;
            inline void             clear() noexcept;


    private:
            inline void             allocateBlock() noexcept;


    private:
            size_t                  mObjectSize;
            size_t                  mAllocationSize;
            float                   mGrowCoefficient;

            Node*                   mBlockList;
            Node*                   mChunkList;
};


#ifdef POOL_ALLOCATOR_IMPL

#include <cstdlib>  /* Allocator uses malloc and free */


PoolAllocator::PoolAllocator(size_t objectSize, size_t initialAllocationSize,
                             float  growCoefficient) noexcept
: mObjectSize(objectSize)
, mAllocationSize(initialAllocationSize)
, mGrowCoefficient(growCoefficient)
, mBlockList(nullptr)
, mChunkList(nullptr)
{
    if (mAllocationSize < 1ULL)
        mAllocationSize = 1ULL;
    if (mGrowCoefficient < 1.f)
        mGrowCoefficient = 1.f;
    if (mObjectSize < sizeof(size_t))  /* Yeah, sizeof(size_t) is the */
        mObjectSize = sizeof(size_t);  /* minimum possible object size */

    allocateBlock();
}

PoolAllocator::~PoolAllocator()
{
    clear();
}

void PoolAllocator::allocateBlock() noexcept
{
    const size_t blockSize = mObjectSize*mAllocationSize + sizeof(Node);
    void* blockPtr = malloc(blockSize);

    ((Node*)blockPtr)->next = mBlockList;
    mBlockList = (Node*)blockPtr;
    const char* lim = (char*)blockPtr + blockSize;
    char* ptr = (char*)blockPtr + sizeof(Node);
    for (; ptr != lim; ptr += mObjectSize)
    {
        ((Node*)ptr)->next = mChunkList;
        mChunkList = (Node*)ptr;
    }
    
    mAllocationSize = mAllocationSize*mGrowCoefficient;
}

void* PoolAllocator::allocate() noexcept
{
    void* ptr = (void*)mChunkList;
    if (not ptr)
    {
        allocateBlock();
        ptr = (void*)mChunkList;
    }
    mChunkList = mChunkList->next;

    return ptr;
}

void PoolAllocator::deallocate(void* ptr) noexcept
{
    ((Node*)ptr)->next = mChunkList;
    mChunkList = (Node*)ptr;
}

void PoolAllocator::clear() noexcept
{
    for (Node* node = mBlockList; node;)
    {
        void* previous = node;
        node = node->next;
        free(previous);
    }

    mBlockList = nullptr;
    mChunkList = nullptr;
}


#endif  /* POOL_ALLOCATOR_IMPL */

#endif  /* POOL_ALLOCATOR_HPP */

