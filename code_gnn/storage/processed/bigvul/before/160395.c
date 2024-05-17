void BaseArena::removeAllPages() {
  clearFreeLists();

  ASSERT(!m_firstUnsweptPage);
  while (m_firstPage) {
    BasePage* page = m_firstPage;
    page->unlink(&m_firstPage);
    page->removeFromHeap();
  }
}
