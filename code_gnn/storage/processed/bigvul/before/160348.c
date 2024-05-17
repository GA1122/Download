Address NormalPageArena::allocateFromFreeList(size_t allocationSize,
                                              size_t gcInfoIndex) {
  size_t bucketSize = static_cast<size_t>(1)
                      << m_freeList.m_biggestFreeListIndex;
  int index = m_freeList.m_biggestFreeListIndex;
  for (; index > 0; --index, bucketSize >>= 1) {
    FreeListEntry* entry = m_freeList.m_freeLists[index];
    if (allocationSize > bucketSize) {
      if (!entry || entry->size() < allocationSize)
        break;
    }
    if (entry) {
      entry->unlink(&m_freeList.m_freeLists[index]);
      setAllocationPoint(entry->getAddress(), entry->size());
      ASSERT(hasCurrentAllocationArea());
      ASSERT(remainingAllocationSize() >= allocationSize);
      m_freeList.m_biggestFreeListIndex = index;
      return allocateObject(allocationSize, gcInfoIndex);
    }
  }
  m_freeList.m_biggestFreeListIndex = index;
  return nullptr;
}
