void BaseArena::sweepUnsweptPage() {
  BasePage* page = m_firstUnsweptPage;
  if (page->isEmpty()) {
    page->unlink(&m_firstUnsweptPage);
    page->removeFromHeap();
  } else {
    page->sweep();
    page->unlink(&m_firstUnsweptPage);
    page->link(&m_firstPage);
    page->markAsSwept();
  }
}
