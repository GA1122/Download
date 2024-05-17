Address BaseArena::allocateLargeObject(size_t allocationSize,
                                       size_t gcInfoIndex) {
  CHECK(arenaIndex() != BlinkGC::EagerSweepArenaIndex);
  LargeObjectArena* largeObjectArena = static_cast<LargeObjectArena*>(
      getThreadState()->arena(BlinkGC::LargeObjectArenaIndex));
  Address largeObject =
      largeObjectArena->allocateLargeObjectPage(allocationSize, gcInfoIndex);
  ASAN_MARK_LARGE_VECTOR_CONTAINER(this, largeObject);
  return largeObject;
}
