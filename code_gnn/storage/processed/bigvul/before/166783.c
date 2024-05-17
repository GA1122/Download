bool FreeList::TakeSnapshot(const String& dump_base_name) {
  bool did_dump_bucket_stats = false;
  for (size_t i = 0; i < kBlinkPageSizeLog2; ++i) {
    size_t entry_count = 0;
    size_t free_size = 0;
    for (FreeListEntry* entry = free_lists_[i]; entry; entry = entry->Next()) {
      ++entry_count;
      free_size += entry->size();
    }

    String dump_name =
        dump_base_name + String::Format("/buckets/bucket_%lu",
                                        static_cast<unsigned long>(1 << i));
    base::trace_event::MemoryAllocatorDump* bucket_dump =
        BlinkGCMemoryDumpProvider::Instance()
            ->CreateMemoryAllocatorDumpForCurrentGC(dump_name);
    bucket_dump->AddScalar("free_count", "objects", entry_count);
    bucket_dump->AddScalar("free_size", "bytes", free_size);
    did_dump_bucket_stats = true;
  }
  return did_dump_bucket_stats;
}
