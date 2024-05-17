size_t NormalPageArena::arenaSize() {
  size_t size = 0;
  BasePage* page = m_firstPage;
  while (page) {
    size += page->size();
    page = page->next();
  }
  LOG_HEAP_FREELIST_VERBOSE("Heap size: %zu (%d)\n", size, arenaIndex());
  return size;
}
