GlobalHistogramAllocator::GlobalHistogramAllocator(
    std::unique_ptr<PersistentMemoryAllocator> memory)
    : PersistentHistogramAllocator(std::move(memory)),
      import_iterator_(this) {
}
