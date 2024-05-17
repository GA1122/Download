void BaseArena::makeConsistentForGC() {
  clearFreeLists();
  ASSERT(isConsistentForGC());
  for (BasePage* page = m_firstPage; page; page = page->next()) {
    page->markAsUnswept();
    page->invalidateObjectStartBitmap();
  }

  CHECK(!m_firstUnsweptPage);

  HeapCompact* heapCompactor = getThreadState()->heap().compaction();
  if (!heapCompactor->isCompactingArena(arenaIndex()))
    return;

  BasePage* nextPage = m_firstPage;
  while (nextPage) {
    if (!nextPage->isLargeObjectPage())
      heapCompactor->addCompactingPage(nextPage);
    nextPage = nextPage->next();
  }
}
