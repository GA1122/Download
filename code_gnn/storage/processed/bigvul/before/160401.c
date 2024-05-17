void NormalPageArena::sweepAndCompact() {
  ThreadHeap& heap = getThreadState()->heap();
  if (!heap.compaction()->isCompactingArena(arenaIndex()))
    return;

  if (!m_firstUnsweptPage) {
    heap.compaction()->finishedArenaCompaction(this, 0, 0);
    return;
  }

  NormalPage::CompactionContext context;
  context.m_compactedPages = &m_firstPage;

  while (m_firstUnsweptPage) {
    BasePage* page = m_firstUnsweptPage;
    if (page->isEmpty()) {
      page->unlink(&m_firstUnsweptPage);
      page->removeFromHeap();
      continue;
    }
    DCHECK(!page->isLargeObjectPage());
    NormalPage* normalPage = static_cast<NormalPage*>(page);
    normalPage->unlink(&m_firstUnsweptPage);
    normalPage->markAsSwept();
    if (!context.m_currentPage)
      context.m_currentPage = normalPage;
    else
      normalPage->link(&context.m_availablePages);
    normalPage->sweepAndCompact(context);
  }

  size_t freedSize = 0;
  size_t freedPageCount = 0;

  DCHECK(context.m_currentPage);
  size_t allocationPoint = context.m_allocationPoint;
  if (!allocationPoint) {
    context.m_currentPage->link(&context.m_availablePages);
  } else {
    NormalPage* currentPage = context.m_currentPage;
    currentPage->link(&m_firstPage);
    if (allocationPoint != currentPage->payloadSize()) {
      freedSize = currentPage->payloadSize() - allocationPoint;
      Address payload = currentPage->payload();
      SET_MEMORY_INACCESSIBLE(payload + allocationPoint, freedSize);
      currentPage->arenaForNormalPage()->addToFreeList(
          payload + allocationPoint, freedSize);
    }
  }

  BasePage* availablePages = context.m_availablePages;
  while (availablePages) {
    size_t pageSize = availablePages->size();
#if DEBUG_HEAP_COMPACTION
    if (!freedPageCount)
      LOG_HEAP_COMPACTION("Releasing:");
    LOG_HEAP_COMPACTION(" [%p, %p]", availablePages, availablePages + pageSize);
#endif
    freedSize += pageSize;
    freedPageCount++;
    BasePage* nextPage;
    availablePages->unlink(&nextPage);
#if !(DCHECK_IS_ON() || defined(LEAK_SANITIZER) || \
      defined(ADDRESS_SANITIZER) || defined(MEMORY_SANITIZER))
    DCHECK(!availablePages->isLargeObjectPage());
    NormalPage* unusedPage = reinterpret_cast<NormalPage*>(availablePages);
    memset(unusedPage->payload(), 0, unusedPage->payloadSize());
#endif
    availablePages->removeFromHeap();
    availablePages = static_cast<NormalPage*>(nextPage);
  }
  if (freedPageCount)
    LOG_HEAP_COMPACTION("\n");
  heap.compaction()->finishedArenaCompaction(this, freedPageCount, freedSize);
}
