void BaseArena::MakeConsistentForGC() {
#if DCHECK_IS_ON()
  DCHECK(IsConsistentForGC());
#endif

  CHECK(SweepingCompleted());

  HeapCompact* heap_compactor = GetThreadState()->Heap().Compaction();
  if (!heap_compactor->IsCompactingArena(ArenaIndex()))
    return;

  BasePage* next_page = first_page_;
  while (next_page) {
    if (!next_page->IsLargeObjectPage())
      heap_compactor->AddCompactingPage(next_page);
    next_page = next_page->Next();
  }
}
