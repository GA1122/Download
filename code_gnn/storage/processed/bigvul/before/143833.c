  std::unique_ptr<base::PersistentHistogramAllocator> CreateDuplicateAllocator(
      base::PersistentHistogramAllocator* allocator) {
    return std::make_unique<base::PersistentHistogramAllocator>(
        std::make_unique<base::PersistentMemoryAllocator>(
            const_cast<void*>(allocator->data()), allocator->length(), 0, 0,
            std::string(), false));
  }
