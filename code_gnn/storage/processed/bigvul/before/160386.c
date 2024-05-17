size_t BaseArena::objectPayloadSizeForTesting() {
  ASSERT(isConsistentForGC());
  ASSERT(!m_firstUnsweptPage);

  size_t objectPayloadSize = 0;
  for (BasePage* page = m_firstPage; page; page = page->next())
    objectPayloadSize += page->objectPayloadSizeForTesting();
  return objectPayloadSize;
}
