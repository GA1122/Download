bool HeapAllocator::expandVectorBacking(void* address, size_t newSize) {
  return backingExpand(address, newSize);
}
