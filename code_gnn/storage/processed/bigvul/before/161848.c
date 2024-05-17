void ClientDiscardableSharedMemoryManager::ReleaseSpan(
    std::unique_ptr<DiscardableSharedMemoryHeap::Span> span) {
  base::AutoLock lock(lock_);

  if (!span->shared_memory())
    return;

  heap_->MergeIntoFreeLists(std::move(span));

  MemoryUsageChanged(heap_->GetSize(), heap_->GetSizeOfFreeLists());
}
