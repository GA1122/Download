void HeapAllocator::freeInlineVectorBacking(void* address) {
  backingFree(address);
}
