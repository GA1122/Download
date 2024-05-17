bool HeapAllocator::shrinkVectorBacking(void* address,
                                        size_t quantizedCurrentSize,
                                        size_t quantizedShrunkSize) {
  return backingShrink(address, quantizedCurrentSize, quantizedShrunkSize);
}
