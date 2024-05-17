size_t FreeList::freeListSize() const {
  size_t freeSize = 0;
  for (unsigned i = 0; i < blinkPageSizeLog2; ++i) {
    FreeListEntry* entry = m_freeLists[i];
    while (entry) {
      freeSize += entry->size();
      entry = entry->next();
    }
  }
#if DEBUG_HEAP_FREELIST
  if (freeSize) {
    LOG_HEAP_FREELIST_VERBOSE("FreeList(%p): %zu\n", this, freeSize);
    for (unsigned i = 0; i < blinkPageSizeLog2; ++i) {
      FreeListEntry* entry = m_freeLists[i];
      size_t bucket = 0;
      size_t count = 0;
      while (entry) {
        bucket += entry->size();
        count++;
        entry = entry->next();
      }
      if (bucket) {
        LOG_HEAP_FREELIST_VERBOSE("[%d, %d]: %zu (%zu)\n", 0x1 << i,
                                  0x1 << (i + 1), bucket, count);
      }
    }
  }
#endif
  return freeSize;
}
