void NormalPageArena::SweepAndCompact() {
  ThreadHeap& heap = GetThreadState()->Heap();
  if (!heap.Compaction()->IsCompactingArena(ArenaIndex()))
    return;

  if (SweepingCompleted()) {
    heap.Compaction()->FinishedArenaCompaction(this, 0, 0);
    return;
  }

  NormalPage::CompactionContext context;
  context.compacted_pages_ = &first_page_;

  while (!SweepingCompleted()) {
    BasePage* page = first_unswept_page_;
    if (page->IsEmpty()) {
      page->Unlink(&first_unswept_page_);
      page->RemoveFromHeap();
      continue;
    }
    DCHECK(!page->IsLargeObjectPage());
    NormalPage* normal_page = static_cast<NormalPage*>(page);
    normal_page->Unlink(&first_unswept_page_);
    normal_page->MarkAsSwept();
    if (!context.current_page_)
      context.current_page_ = normal_page;
    else
      normal_page->Link(&context.available_pages_);
    normal_page->SweepAndCompact(context);
  }

  if (!context.current_page_) {
    heap.Compaction()->FinishedArenaCompaction(this, 0, 0);
    return;
  }

  size_t freed_size = 0;
  size_t freed_page_count = 0;

  size_t allocation_point = context.allocation_point_;
  if (!allocation_point) {
    context.current_page_->Link(&context.available_pages_);
  } else {
    NormalPage* current_page = context.current_page_;
    current_page->Link(&first_page_);
    if (allocation_point != current_page->PayloadSize()) {
      freed_size = current_page->PayloadSize() - allocation_point;
      Address payload = current_page->Payload();
      SET_MEMORY_INACCESSIBLE(payload + allocation_point, freed_size);
      current_page->ArenaForNormalPage()->AddToFreeList(
          payload + allocation_point, freed_size);
    }
  }

  BasePage* available_pages = context.available_pages_;
#if DEBUG_HEAP_COMPACTION
  std::stringstream stream;
#endif
  while (available_pages) {
    size_t page_size = available_pages->size();
#if DEBUG_HEAP_COMPACTION
    if (!freed_page_count)
      stream << "Releasing:";
    stream << " [" << available_pages << ", " << (available_pages + page_size)
           << "]";
#endif
    freed_size += page_size;
    freed_page_count++;
    BasePage* next_page;
    available_pages->Unlink(&next_page);
#if !(DCHECK_IS_ON() || defined(LEAK_SANITIZER) || \
      defined(ADDRESS_SANITIZER) || defined(MEMORY_SANITIZER))
    DCHECK(!available_pages->IsLargeObjectPage());
    NormalPage* unused_page = reinterpret_cast<NormalPage*>(available_pages);
    memset(unused_page->Payload(), 0, unused_page->PayloadSize());
#endif
    available_pages->RemoveFromHeap();
    available_pages = static_cast<NormalPage*>(next_page);
  }
#if DEBUG_HEAP_COMPACTION
  if (freed_page_count)
    LOG_HEAP_COMPACTION() << stream.str();
#endif
  heap.Compaction()->FinishedArenaCompaction(this, freed_page_count,
                                             freed_size);

  VerifyObjectStartBitmap();
}
