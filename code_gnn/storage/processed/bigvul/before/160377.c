Address NormalPageArena::lazySweepPages(size_t allocationSize,
                                        size_t gcInfoIndex) {
  ASSERT(!hasCurrentAllocationArea());
  AutoReset<bool> isLazySweeping(&m_isLazySweeping, true);
  Address result = nullptr;
  while (m_firstUnsweptPage) {
    BasePage* page = m_firstUnsweptPage;
    if (page->isEmpty()) {
      page->unlink(&m_firstUnsweptPage);
      page->removeFromHeap();
    } else {
      page->sweep();
      page->unlink(&m_firstUnsweptPage);
      page->link(&m_firstPage);
      page->markAsSwept();

      result = allocateFromFreeList(allocationSize, gcInfoIndex);
      if (result)
        break;
    }
  }
  return result;
}
