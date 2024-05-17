bool ClientDiscardableSharedMemoryManager::OnMemoryDump(
    const base::trace_event::MemoryDumpArgs& args,
    base::trace_event::ProcessMemoryDump* pmd) {
  base::AutoLock lock(lock_);
  if (args.level_of_detail ==
      base::trace_event::MemoryDumpLevelOfDetail::BACKGROUND) {
    base::trace_event::MemoryAllocatorDump* total_dump =
        pmd->CreateAllocatorDump(
            base::StringPrintf("discardable/child_0x%" PRIXPTR,
                               reinterpret_cast<uintptr_t>(this)));
    const size_t total_size = heap_->GetSize();
    const size_t freelist_size = heap_->GetSizeOfFreeLists();
    total_dump->AddScalar(base::trace_event::MemoryAllocatorDump::kNameSize,
                          base::trace_event::MemoryAllocatorDump::kUnitsBytes,
                          total_size - freelist_size);
    total_dump->AddScalar("freelist_size",
                          base::trace_event::MemoryAllocatorDump::kUnitsBytes,
                          freelist_size);
    return true;
  }

  return heap_->OnMemoryDump(pmd);
}
