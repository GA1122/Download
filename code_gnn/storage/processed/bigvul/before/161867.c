bool DiscardableSharedMemoryManager::OnMemoryDump(
    const base::trace_event::MemoryDumpArgs& args,
    base::trace_event::ProcessMemoryDump* pmd) {
  if (args.level_of_detail ==
      base::trace_event::MemoryDumpLevelOfDetail::BACKGROUND) {
    base::trace_event::MemoryAllocatorDump* total_dump =
        pmd->CreateAllocatorDump("discardable");
    total_dump->AddScalar(base::trace_event::MemoryAllocatorDump::kNameSize,
                          base::trace_event::MemoryAllocatorDump::kUnitsBytes,
                          GetBytesAllocated());
    return true;
  }

  base::AutoLock lock(lock_);
  for (const auto& client_entry : clients_) {
    const int client_id = client_entry.first;
    const MemorySegmentMap& client_segments = client_entry.second;
    for (const auto& segment_entry : client_segments) {
      const int segment_id = segment_entry.first;
      const MemorySegment* segment = segment_entry.second.get();
      if (!segment->memory()->mapped_size())
        continue;

      std::string dump_name = base::StringPrintf(
          "discardable/process_%x/segment_%d", client_id, segment_id);
      base::trace_event::MemoryAllocatorDump* dump =
          pmd->CreateAllocatorDump(dump_name);

      dump->AddScalar("virtual_size",
                      base::trace_event::MemoryAllocatorDump::kUnitsBytes,
                      segment->memory()->mapped_size());

      dump->AddScalar(
          "locked_size", base::trace_event::MemoryAllocatorDump::kUnitsBytes,
          segment->memory()->IsMemoryLocked() ? segment->memory()->mapped_size()
                                              : 0u);

      segment->memory()->CreateSharedMemoryOwnershipEdge(dump, pmd,
                                                          false);
    }
  }
  return true;
}
