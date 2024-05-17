Address BaseArena::AllocateLargeObject(size_t allocation_size,
                                       size_t gc_info_index) {
  CHECK(ArenaIndex() != BlinkGC::kEagerSweepArenaIndex);
  LargeObjectArena* large_object_arena = static_cast<LargeObjectArena*>(
      GetThreadState()->Heap().Arena(BlinkGC::kLargeObjectArenaIndex));
  Address large_object = large_object_arena->AllocateLargeObjectPage(
      allocation_size, gc_info_index);
  ASAN_MARK_LARGE_VECTOR_CONTAINER(this, large_object);
  return large_object;
}
