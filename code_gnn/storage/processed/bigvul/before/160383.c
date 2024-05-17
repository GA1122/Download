void NormalPage::makeConsistentForMutator() {
  Address startOfGap = payload();
  NormalPageArena* normalArena = arenaForNormalPage();
  for (Address headerAddress = payload(); headerAddress < payloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(headerAddress);
    size_t size = header->size();
    ASSERT(size < blinkPagePayloadSize());
    if (header->isPromptlyFreed())
      arenaForNormalPage()->decreasePromptlyFreedSize(size);
    if (header->isFree()) {
      SET_MEMORY_INACCESSIBLE(headerAddress, size < sizeof(FreeListEntry)
                                                 ? size
                                                 : sizeof(FreeListEntry));
      CHECK_MEMORY_INACCESSIBLE(headerAddress, size);
      headerAddress += size;
      continue;
    }
    if (startOfGap != headerAddress)
      normalArena->addToFreeList(startOfGap, headerAddress - startOfGap);
    if (header->isMarked())
      header->unmark();
    headerAddress += size;
    startOfGap = headerAddress;
    ASSERT(headerAddress <= payloadEnd());
  }
  if (startOfGap != payloadEnd())
    normalArena->addToFreeList(startOfGap, payloadEnd() - startOfGap);
}
