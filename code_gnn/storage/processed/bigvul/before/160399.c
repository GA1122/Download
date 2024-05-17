void NormalPage::sweep() {
  size_t markedObjectSize = 0;
  Address startOfGap = payload();
  NormalPageArena* pageArena = arenaForNormalPage();
  for (Address headerAddress = startOfGap; headerAddress < payloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(headerAddress);
    size_t size = header->size();
    ASSERT(size > 0);
    ASSERT(size < blinkPagePayloadSize());

    if (header->isPromptlyFreed())
      pageArena->decreasePromptlyFreedSize(size);
    if (header->isFree()) {
      SET_MEMORY_INACCESSIBLE(headerAddress, size < sizeof(FreeListEntry)
                                                 ? size
                                                 : sizeof(FreeListEntry));
      CHECK_MEMORY_INACCESSIBLE(headerAddress, size);
      headerAddress += size;
      continue;
    }
    if (!header->isMarked()) {
      size_t payloadSize = size - sizeof(HeapObjectHeader);
      Address payload = header->payload();
      ASAN_UNPOISON_MEMORY_REGION(payload, payloadSize);
      header->finalize(payload, payloadSize);
      SET_MEMORY_INACCESSIBLE(headerAddress, size);
      headerAddress += size;
      continue;
    }
    if (startOfGap != headerAddress) {
      pageArena->addToFreeList(startOfGap, headerAddress - startOfGap);
#if !DCHECK_IS_ON() && !defined(LEAK_SANITIZER) && !defined(ADDRESS_SANITIZER)
      if (MemoryCoordinator::isLowEndDevice())
        discardPages(startOfGap + sizeof(FreeListEntry), headerAddress);
#endif
    }
    header->unmark();
    headerAddress += size;
    markedObjectSize += size;
    startOfGap = headerAddress;
  }
  if (startOfGap != payloadEnd()) {
    pageArena->addToFreeList(startOfGap, payloadEnd() - startOfGap);
#if !DCHECK_IS_ON() && !defined(LEAK_SANITIZER) && !defined(ADDRESS_SANITIZER)
    if (MemoryCoordinator::isLowEndDevice())
      discardPages(startOfGap + sizeof(FreeListEntry), payloadEnd());
#endif
  }

  if (markedObjectSize)
    pageArena->getThreadState()->increaseMarkedObjectSize(markedObjectSize);
}
