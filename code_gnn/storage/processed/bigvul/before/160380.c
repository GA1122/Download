bool HeapDoesNotContainCache::lookup(Address address) {
  ASSERT(ThreadState::current()->isInGC());

  size_t index = hash(address);
  ASSERT(!(index & 1));
  Address cachePage = roundToBlinkPageStart(address);
  if (m_entries[index] == cachePage)
    return m_entries[index];
  if (m_entries[index + 1] == cachePage)
    return m_entries[index + 1];
  return false;
}
