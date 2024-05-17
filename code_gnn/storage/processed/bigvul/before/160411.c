BaseArena::~BaseArena() {
  ASSERT(!m_firstPage);
  ASSERT(!m_firstUnsweptPage);
}
