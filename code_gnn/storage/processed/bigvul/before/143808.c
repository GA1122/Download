PersistentHistogramAllocator::PersistentHistogramAllocator(
    std::unique_ptr<PersistentMemoryAllocator> memory)
    : memory_allocator_(std::move(memory)),
      sparse_histogram_data_manager_(memory_allocator_.get()) {}
