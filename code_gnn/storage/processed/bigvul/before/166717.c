void FreeList::AddToFreeList(Address address, size_t size) {
  DCHECK_LT(size, BlinkPagePayloadSize());
  DCHECK(!((reinterpret_cast<uintptr_t>(address) + sizeof(HeapObjectHeader)) &
           kAllocationMask));
  DCHECK(!(size & kAllocationMask));
  ASAN_UNPOISON_MEMORY_REGION(address, size);
  FreeListEntry* entry;
  if (size < sizeof(*entry)) {
    DCHECK_GE(size, sizeof(HeapObjectHeader));
    new (NotNull, address) HeapObjectHeader(size, kGcInfoIndexForFreeListHeader,
                                            HeapObjectHeader::kNormalPage);

    ASAN_POISON_MEMORY_REGION(address, size);
    return;
  }
  entry = new (NotNull, address) FreeListEntry(size);

#if DCHECK_IS_ON() || defined(LEAK_SANITIZER) || defined(ADDRESS_SANITIZER)
  size_t allowed_count = 0;
  size_t forbidden_count = 0;
  GetAllowedAndForbiddenCounts(address, size, allowed_count, forbidden_count);
  size_t entry_count = size - sizeof(FreeListEntry);
  if (forbidden_count == entry_count) {
    for (size_t i = sizeof(FreeListEntry); i < size; i++)
      address[i] = kReuseAllowedZapValue;
    ASAN_POISON_MEMORY_REGION(address, size);
    return;
  }
  if (allowed_count != entry_count) {
    for (size_t i = sizeof(FreeListEntry); i < size; i++)
      address[i] = kReuseForbiddenZapValue;
    ASAN_POISON_MEMORY_REGION(address, size);
    return;
  }
#endif
  ASAN_POISON_MEMORY_REGION(address, size);

  int index = BucketIndexForSize(size);
  entry->Link(&free_lists_[index]);
  if (index > biggest_free_list_index_)
    biggest_free_list_index_ = index;
}
