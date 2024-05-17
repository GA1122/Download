void BaseArena::takeSnapshot(const String& dumpBaseName,
                             ThreadState::GCSnapshotInfo& info) {
  base::trace_event::MemoryAllocatorDump* allocatorDump =
      BlinkGCMemoryDumpProvider::instance()
          ->createMemoryAllocatorDumpForCurrentGC(dumpBaseName);
  size_t pageCount = 0;
  BasePage::HeapSnapshotInfo heapInfo;
  for (BasePage* page = m_firstUnsweptPage; page; page = page->next()) {
    String dumpName =
        dumpBaseName + String::format("/pages/page_%lu",
                                      static_cast<unsigned long>(pageCount++));
    base::trace_event::MemoryAllocatorDump* pageDump =
        BlinkGCMemoryDumpProvider::instance()
            ->createMemoryAllocatorDumpForCurrentGC(dumpName);

    page->takeSnapshot(pageDump, info, heapInfo);
  }
  allocatorDump->AddScalar("blink_page_count", "objects", pageCount);

  allocatorDump->AddScalar("free_size", "bytes", heapInfo.freeSize);
  allocatorDump->AddScalar("free_count", "objects", heapInfo.freeCount);
}
