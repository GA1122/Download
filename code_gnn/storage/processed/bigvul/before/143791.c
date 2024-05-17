void GlobalHistogramAllocator::CreateWithPersistentMemory(
    void* base,
    size_t size,
    size_t page_size,
    uint64_t id,
    StringPiece name) {
  Set(WrapUnique(
      new GlobalHistogramAllocator(std::make_unique<PersistentMemoryAllocator>(
          base, size, page_size, id, name, false))));
}
