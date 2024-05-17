void HeapDoesNotContainCache::flush() {
  if (m_hasEntries) {
    for (int i = 0; i < numberOfEntries; ++i)
      m_entries[i] = nullptr;
    m_hasEntries = false;
  }
}
