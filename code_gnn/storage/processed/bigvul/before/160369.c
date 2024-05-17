size_t NormalPageArena::freeListSize() {
  size_t freeSize = m_freeList.freeListSize();
  LOG_HEAP_FREELIST_VERBOSE("Free size: %zu (%d)\n", freeSize, arenaIndex());
  return freeSize;
}
