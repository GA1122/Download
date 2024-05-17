void BaseArena::makeConsistentForMutator() {
  clearFreeLists();
  ASSERT(isConsistentForGC());
  ASSERT(!m_firstPage);

  BasePage* previousPage = nullptr;
  for (BasePage *page = m_firstUnsweptPage; page;
       previousPage = page, page = page->next()) {
    page->makeConsistentForMutator();
    page->markAsSwept();
    page->invalidateObjectStartBitmap();
  }
  if (previousPage) {
    ASSERT(m_firstUnsweptPage);
    previousPage->m_next = m_firstPage;
    m_firstPage = m_firstUnsweptPage;
    m_firstUnsweptPage = nullptr;
  }
  ASSERT(!m_firstUnsweptPage);
}
