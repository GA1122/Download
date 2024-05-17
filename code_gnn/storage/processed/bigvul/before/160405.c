bool FreeList::takeSnapshot(const String& dumpBaseName) {
  bool didDumpBucketStats = false;
  for (size_t i = 0; i < blinkPageSizeLog2; ++i) {
    size_t entryCount = 0;
    size_t freeSize = 0;
    for (FreeListEntry* entry = m_freeLists[i]; entry; entry = entry->next()) {
      ++entryCount;
      freeSize += entry->size();
    }

    String dumpName =
        dumpBaseName + String::format("/buckets/bucket_%lu",
                                      static_cast<unsigned long>(1 << i));
    base::trace_event::MemoryAllocatorDump* bucketDump =
        BlinkGCMemoryDumpProvider::instance()
            ->createMemoryAllocatorDumpForCurrentGC(dumpName);
    bucketDump->AddScalar("free_count", "objects", entryCount);
    bucketDump->AddScalar("free_size", "bytes", freeSize);
    didDumpBucketStats = true;
  }
  return didDumpBucketStats;
}
