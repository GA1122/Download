size_t FreeList::FreeListSize() const {
  size_t free_size = 0;
  for (unsigned i = 0; i < kBlinkPageSizeLog2; ++i) {
    FreeListEntry* entry = free_lists_[i];
    while (entry) {
      free_size += entry->size();
      entry = entry->Next();
    }
  }
#if DEBUG_HEAP_FREELIST
  if (free_size) {
    LOG_HEAP_FREELIST_VERBOSE() << "FreeList(" << this << "): " << free_size;
    for (unsigned i = 0; i < kBlinkPageSizeLog2; ++i) {
      FreeListEntry* entry = free_lists_[i];
      size_t bucket = 0;
      size_t count = 0;
      while (entry) {
        bucket += entry->size();
        count++;
        entry = entry->Next();
      }
      if (bucket) {
        LOG_HEAP_FREELIST_VERBOSE()
            << "[" << (0x1 << i) << ", " << (0x1 << (i + 1)) << "]: " << bucket
            << " (" << count << ")";
      }
    }
  }
#endif
  return free_size;
}
