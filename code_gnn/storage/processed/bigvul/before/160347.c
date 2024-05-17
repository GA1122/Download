void FreeList::addToFreeList(Address address, size_t size) {
  ASSERT(size < blinkPagePayloadSize());
  ASSERT(!((reinterpret_cast<uintptr_t>(address) + sizeof(HeapObjectHeader)) &
           allocationMask));
  ASSERT(!(size & allocationMask));
  ASAN_UNPOISON_MEMORY_REGION(address, size);
  FreeListEntry* entry;
  if (size < sizeof(*entry)) {
    ASSERT(size >= sizeof(HeapObjectHeader));
    new (NotNull, address) HeapObjectHeader(size, gcInfoIndexForFreeListHeader);

    ASAN_POISON_MEMORY_REGION(address, size);
    return;
  }
  entry = new (NotNull, address) FreeListEntry(size);

#if DCHECK_IS_ON() || defined(LEAK_SANITIZER) || defined(ADDRESS_SANITIZER)
  size_t allowedCount = 0;
  size_t forbiddenCount = 0;
  getAllowedAndForbiddenCounts(address, size, allowedCount, forbiddenCount);
  size_t entryCount = size - sizeof(FreeListEntry);
  if (forbiddenCount == entryCount) {
    for (size_t i = sizeof(FreeListEntry); i < size; i++)
      address[i] = reuseAllowedZapValue;
    ASAN_POISON_MEMORY_REGION(address, size);
    return;
  }
  if (allowedCount != entryCount) {
    for (size_t i = sizeof(FreeListEntry); i < size; i++)
      address[i] = reuseForbiddenZapValue;
    ASAN_POISON_MEMORY_REGION(address, size);
    return;
  }
#endif
  ASAN_POISON_MEMORY_REGION(address, size);

  int index = bucketIndexForSize(size);
  entry->link(&m_freeLists[index]);
  if (index > m_biggestFreeListIndex)
    m_biggestFreeListIndex = index;
}
