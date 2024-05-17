bool HeapAllocator::expandInlineVectorBacking(void* address, size_t newSize) {
  return backingExpand(address, newSize);
}
