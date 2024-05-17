void ClientDiscardableSharedMemoryManager::ReleaseFreeMemory() {
  base::AutoLock lock(lock_);

  size_t heap_size_prior_to_releasing_memory = heap_->GetSize();

  heap_->ReleasePurgedMemory();
  heap_->ReleaseFreeMemory();

  if (heap_->GetSize() != heap_size_prior_to_releasing_memory)
    MemoryUsageChanged(heap_->GetSize(), heap_->GetSizeOfFreeLists());
}
