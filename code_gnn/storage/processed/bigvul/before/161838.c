ClientDiscardableSharedMemoryManager::CreateMemoryAllocatorDump(
    DiscardableSharedMemoryHeap::Span* span,
    const char* name,
    base::trace_event::ProcessMemoryDump* pmd) const {
  base::AutoLock lock(lock_);
  return heap_->CreateMemoryAllocatorDump(span, name, pmd);
}
