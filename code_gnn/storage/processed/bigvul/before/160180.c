size_t MemBackendImpl::DumpMemoryStats(
    base::trace_event::ProcessMemoryDump* pmd,
    const std::string& parent_absolute_name) const {
  base::trace_event::MemoryAllocatorDump* dump =
      pmd->CreateAllocatorDump(parent_absolute_name + "/memory_backend");

  size_t size = base::trace_event::EstimateMemoryUsage(lru_list_) +
                base::trace_event::EstimateMemoryUsage(entries_);
  dump->AddScalar(base::trace_event::MemoryAllocatorDump::kNameSize,
                  base::trace_event::MemoryAllocatorDump::kUnitsBytes, size);
  dump->AddScalar("mem_backend_size",
                  base::trace_event::MemoryAllocatorDump::kUnitsBytes,
                  current_size_);
  dump->AddScalar("mem_backend_max_size",
                  base::trace_event::MemoryAllocatorDump::kUnitsBytes,
                  max_size_);
  return size;
}
