PersistentSparseHistogramDataManager::PersistentSparseHistogramDataManager(
    PersistentMemoryAllocator* allocator)
    : allocator_(allocator), record_iterator_(allocator) {}
