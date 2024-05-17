void GlobalHistogramAllocator::CreateWithLocalMemory(
    size_t size,
    uint64_t id,
    StringPiece name) {
  Set(WrapUnique(new GlobalHistogramAllocator(
      std::make_unique<LocalPersistentMemoryAllocator>(size, id, name))));
}
