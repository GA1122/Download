ClientDiscardableSharedMemoryManager::AllocateLockedDiscardableMemory(
    size_t size) {
  base::AutoLock lock(lock_);

  DCHECK_NE(size, 0u);

  auto size_in_kb = static_cast<base::HistogramBase::Sample>(size / 1024);
  UMA_HISTOGRAM_CUSTOM_COUNTS("Memory.DiscardableAllocationSize",
                              size_in_kb,   
                              1,
                              4 * 1024 * 1024,   
                              50);

  size_t pages =
      std::max((size + base::GetPageSize() - 1) / base::GetPageSize(),
               static_cast<size_t>(1));

  size_t allocation_pages = kAllocationSize / base::GetPageSize();

  size_t slack = 0;
  if (pages < allocation_pages)
    slack = allocation_pages - pages;

  size_t heap_size_prior_to_releasing_purged_memory = heap_->GetSize();
  for (;;) {
    std::unique_ptr<DiscardableSharedMemoryHeap::Span> free_span =
        heap_->SearchFreeLists(pages, slack);
    if (!free_span.get())
      break;

    if (free_span->shared_memory()->Lock(
            free_span->start() * base::GetPageSize() -
                reinterpret_cast<size_t>(free_span->shared_memory()->memory()),
            free_span->length() * base::GetPageSize()) ==
        base::DiscardableSharedMemory::FAILED) {
      DCHECK(!free_span->shared_memory()->IsMemoryResident());
      heap_->ReleasePurgedMemory();
      DCHECK(!free_span->shared_memory());
      continue;
    }

    free_span->set_is_locked(true);

    MemoryUsageChanged(heap_->GetSize(), heap_->GetSizeOfFreeLists());

    return base::MakeUnique<DiscardableMemoryImpl>(this, std::move(free_span));
  }

  heap_->ReleasePurgedMemory();

  if (heap_->GetSize() != heap_size_prior_to_releasing_purged_memory)
    MemoryUsageChanged(heap_->GetSize(), heap_->GetSizeOfFreeLists());

  size_t pages_to_allocate =
      std::max(kAllocationSize / base::GetPageSize(), pages);
  size_t allocation_size_in_bytes = pages_to_allocate * base::GetPageSize();

  int32_t new_id = g_next_discardable_shared_memory_id.GetNext();

  std::unique_ptr<base::DiscardableSharedMemory> shared_memory =
      AllocateLockedDiscardableSharedMemory(allocation_size_in_bytes, new_id);

  std::unique_ptr<DiscardableSharedMemoryHeap::Span> new_span(heap_->Grow(
      std::move(shared_memory), allocation_size_in_bytes, new_id,
      base::Bind(
          &ClientDiscardableSharedMemoryManager::DeletedDiscardableSharedMemory,
          base::Unretained(this), new_id)));
  new_span->set_is_locked(true);

  if (pages < pages_to_allocate) {
    std::unique_ptr<DiscardableSharedMemoryHeap::Span> leftover =
        heap_->Split(new_span.get(), pages);
    leftover->shared_memory()->Unlock(
        leftover->start() * base::GetPageSize() -
            reinterpret_cast<size_t>(leftover->shared_memory()->memory()),
        leftover->length() * base::GetPageSize());
    leftover->set_is_locked(false);
    heap_->MergeIntoFreeLists(std::move(leftover));
  }

  MemoryUsageChanged(heap_->GetSize(), heap_->GetSizeOfFreeLists());

  return base::MakeUnique<DiscardableMemoryImpl>(this, std::move(new_span));
}
