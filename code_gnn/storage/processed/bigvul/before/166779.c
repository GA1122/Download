void NormalPage::SweepAndCompact(CompactionContext& context) {
  object_start_bit_map()->Clear();
  NormalPage*& current_page = context.current_page_;
  size_t& allocation_point = context.allocation_point_;

  size_t marked_object_size = 0;
  NormalPageArena* page_arena = ArenaForNormalPage();
#if defined(ADDRESS_SANITIZER)
  bool is_vector_arena =
      ThreadHeap::IsVectorArenaIndex(page_arena->ArenaIndex());
#endif
  HeapCompact* compact = page_arena->GetThreadState()->Heap().Compaction();
  for (Address header_address = Payload(); header_address < PayloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    size_t size = header->size();
    DCHECK_GT(size, 0u);
    DCHECK_LT(size, BlinkPagePayloadSize());

    if (header->IsFree()) {
      ASAN_UNPOISON_MEMORY_REGION(header_address, size);
      header_address += size;
      continue;
    }
    size_t payload_size = size - sizeof(HeapObjectHeader);
    Address payload = header->Payload();
    if (!header->IsMarked()) {
      ASAN_UNPOISON_MEMORY_REGION(header_address, size);
      header->Finalize(payload, payload_size);

#if DCHECK_IS_ON() || defined(LEAK_SANITIZER) || defined(ADDRESS_SANITIZER) || \
    defined(MEMORY_SANITIZER)
      FreeList::ZapFreedMemory(payload, payload_size);
#endif
      header_address += size;
      continue;
    }
    header->Unmark();
    Address compact_frontier = current_page->Payload() + allocation_point;
    if (compact_frontier + size > current_page->PayloadEnd()) {
      current_page->Link(context.compacted_pages_);
      size_t free_size = current_page->PayloadSize() - allocation_point;
      if (free_size) {
        SET_MEMORY_INACCESSIBLE(compact_frontier, free_size);
        current_page->ArenaForNormalPage()->AddToFreeList(compact_frontier,
                                                          free_size);
      }

      BasePage* next_available_page;
      context.available_pages_->Unlink(&next_available_page);
      current_page = reinterpret_cast<NormalPage*>(context.available_pages_);
      context.available_pages_ = next_available_page;
      allocation_point = 0;
      compact_frontier = current_page->Payload();
    }
    if (LIKELY(compact_frontier != header_address)) {
#if defined(ADDRESS_SANITIZER)
      ASAN_UNPOISON_MEMORY_REGION(header, sizeof(HeapObjectHeader));
      if (is_vector_arena)
        ASAN_UNPOISON_MEMORY_REGION(payload, payload_size);
#endif
      if (current_page == this)
        memmove(compact_frontier, header_address, size);
      else
        memcpy(compact_frontier, header_address, size);
      compact->Relocate(payload, compact_frontier + sizeof(HeapObjectHeader));
    }
    current_page->object_start_bit_map()->SetBit(compact_frontier);
    header_address += size;
    marked_object_size += size;
    allocation_point += size;
    DCHECK(allocation_point <= current_page->PayloadSize());
  }
  if (marked_object_size) {
    page_arena->GetThreadState()->Heap().HeapStats().IncreaseMarkedObjectSize(
        marked_object_size);
  }

#if DCHECK_IS_ON() || defined(LEAK_SANITIZER) || defined(ADDRESS_SANITIZER) || \
    defined(MEMORY_SANITIZER)
  if (current_page != this) {
    FreeList::ZapFreedMemory(Payload(), PayloadSize());
  } else {
    FreeList::ZapFreedMemory(Payload() + allocation_point,
                             PayloadSize() - allocation_point);
  }
#endif
}
