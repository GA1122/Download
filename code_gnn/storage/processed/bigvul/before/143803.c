PersistentHistogramAllocator::Iterator::Iterator(
    PersistentHistogramAllocator* allocator)
    : allocator_(allocator), memory_iter_(allocator->memory_allocator()) {}
