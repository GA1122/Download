void NormalPage::Sweep() {
  object_start_bit_map()->Clear();
  size_t marked_object_size = 0;
  Address start_of_gap = Payload();
  NormalPageArena* page_arena = ArenaForNormalPage();
  for (Address header_address = start_of_gap; header_address < PayloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    size_t size = header->size();
    DCHECK_GT(size, 0u);
    DCHECK_LT(size, BlinkPagePayloadSize());

    if (header->IsFree()) {
      SET_MEMORY_INACCESSIBLE(header_address, size < sizeof(FreeListEntry)
                                                  ? size
                                                  : sizeof(FreeListEntry));
      CHECK_MEMORY_INACCESSIBLE(header_address, size);
      header_address += size;
      continue;
    }
    if (!header->IsMarked()) {
      size_t payload_size = size - sizeof(HeapObjectHeader);
      Address payload = header->Payload();
      ASAN_UNPOISON_MEMORY_REGION(payload, payload_size);
      header->Finalize(payload, payload_size);
      SET_MEMORY_INACCESSIBLE(header_address, size);
      header_address += size;
      continue;
    }
    if (start_of_gap != header_address) {
      page_arena->AddToFreeList(start_of_gap, header_address - start_of_gap);
#if !DCHECK_IS_ON() && !defined(LEAK_SANITIZER) && !defined(ADDRESS_SANITIZER)
      if (MemoryCoordinator::IsLowEndDevice())
        DiscardPages(start_of_gap + sizeof(FreeListEntry), header_address);
#endif
    }
    object_start_bit_map()->SetBit(header_address);
    header->Unmark();
    header_address += size;
    marked_object_size += size;
    start_of_gap = header_address;
  }
  if (start_of_gap != PayloadEnd()) {
    page_arena->AddToFreeList(start_of_gap, PayloadEnd() - start_of_gap);
#if !DCHECK_IS_ON() && !defined(LEAK_SANITIZER) && !defined(ADDRESS_SANITIZER)
    if (MemoryCoordinator::IsLowEndDevice())
      DiscardPages(start_of_gap + sizeof(FreeListEntry), PayloadEnd());
#endif
  }

  if (marked_object_size) {
    page_arena->GetThreadState()->Heap().HeapStats().IncreaseMarkedObjectSize(
        marked_object_size);
  }

  VerifyObjectStartBitmapIsConsistentWithPayload();
}
