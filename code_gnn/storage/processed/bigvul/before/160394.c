void BaseArena::prepareForSweep() {
  ASSERT(getThreadState()->isInGC());
  ASSERT(!m_firstUnsweptPage);

  m_firstUnsweptPage = m_firstPage;
  m_firstPage = nullptr;
}
