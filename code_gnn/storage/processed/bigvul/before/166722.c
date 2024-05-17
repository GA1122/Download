size_t NormalPageArena::ArenaSize() {
  size_t size = 0;
  BasePage* page = first_page_;
  while (page) {
    size += page->size();
    page = page->Next();
  }
  LOG_HEAP_FREELIST_VERBOSE()
      << "Heap size: " << size << "(" << ArenaIndex() << ")";
  return size;
}
