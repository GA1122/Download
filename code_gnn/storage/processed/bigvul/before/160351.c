void NormalPageArena::allocatePage() {
  getThreadState()->shouldFlushHeapDoesNotContainCache();
  PageMemory* pageMemory =
      getThreadState()->heap().getFreePagePool()->take(arenaIndex());

  if (!pageMemory) {
    PageMemoryRegion* region = PageMemoryRegion::allocateNormalPages(
        getThreadState()->heap().getRegionTree());

    for (size_t i = 0; i < blinkPagesPerRegion; ++i) {
      PageMemory* memory = PageMemory::setupPageMemoryInRegion(
          region, i * blinkPageSize, blinkPagePayloadSize());
      if (!pageMemory) {
        bool result = memory->commit();
        RELEASE_ASSERT(result);
        pageMemory = memory;
      } else {
        getThreadState()->heap().getFreePagePool()->add(arenaIndex(), memory);
      }
    }
  }
  NormalPage* page =
      new (pageMemory->writableStart()) NormalPage(pageMemory, this);
  page->link(&m_firstPage);

  getThreadState()->heap().heapStats().increaseAllocatedSpace(page->size());
#if DCHECK_IS_ON() || defined(LEAK_SANITIZER) || defined(ADDRESS_SANITIZER)
  ASAN_UNPOISON_MEMORY_REGION(page->payload(), page->payloadSize());
  Address address = page->payload();
  for (size_t i = 0; i < page->payloadSize(); i++)
    address[i] = reuseAllowedZapValue;
  ASAN_POISON_MEMORY_REGION(page->payload(), page->payloadSize());
#endif
  addToFreeList(page->payload(), page->payloadSize());
}
