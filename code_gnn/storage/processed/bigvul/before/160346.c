void HeapDoesNotContainCache::addEntry(Address address) {
  ASSERT(ThreadState::current()->isInGC());

  m_hasEntries = true;
  size_t index = hash(address);
  ASSERT(!(index & 1));
  Address cachePage = roundToBlinkPageStart(address);
  m_entries[index + 1] = m_entries[index];
  m_entries[index] = cachePage;
}
