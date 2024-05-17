size_t NormalPageArena::FreeListSize() {
  size_t free_size = free_list_.FreeListSize();
  LOG_HEAP_FREELIST_VERBOSE()
      << "Free size: " << free_size << "(" << ArenaIndex() << ")";
  return free_size;
}
