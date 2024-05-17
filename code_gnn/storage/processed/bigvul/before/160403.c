void NormalPageArena::takeFreelistSnapshot(const String& dumpName) {
  if (m_freeList.takeSnapshot(dumpName)) {
    base::trace_event::MemoryAllocatorDump* bucketsDump =
        BlinkGCMemoryDumpProvider::instance()
            ->createMemoryAllocatorDumpForCurrentGC(dumpName + "/buckets");
    base::trace_event::MemoryAllocatorDump* pagesDump =
        BlinkGCMemoryDumpProvider::instance()
            ->createMemoryAllocatorDumpForCurrentGC(dumpName + "/pages");
    BlinkGCMemoryDumpProvider::instance()
        ->currentProcessMemoryDump()
        ->AddOwnershipEdge(pagesDump->guid(), bucketsDump->guid());
  }
}
