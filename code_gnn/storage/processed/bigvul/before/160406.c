void NormalPage::takeSnapshot(base::trace_event::MemoryAllocatorDump* pageDump,
                              ThreadState::GCSnapshotInfo& info,
                              HeapSnapshotInfo& heapInfo) {
  HeapObjectHeader* header = nullptr;
  size_t liveCount = 0;
  size_t deadCount = 0;
  size_t freeCount = 0;
  size_t liveSize = 0;
  size_t deadSize = 0;
  size_t freeSize = 0;
  for (Address headerAddress = payload(); headerAddress < payloadEnd();
       headerAddress += header->size()) {
    header = reinterpret_cast<HeapObjectHeader*>(headerAddress);
    if (header->isFree()) {
      freeCount++;
      freeSize += header->size();
    } else if (header->isMarked()) {
      liveCount++;
      liveSize += header->size();

      size_t gcInfoIndex = header->gcInfoIndex();
      info.liveCount[gcInfoIndex]++;
      info.liveSize[gcInfoIndex] += header->size();
    } else {
      deadCount++;
      deadSize += header->size();

      size_t gcInfoIndex = header->gcInfoIndex();
      info.deadCount[gcInfoIndex]++;
      info.deadSize[gcInfoIndex] += header->size();
    }
  }

  pageDump->AddScalar("live_count", "objects", liveCount);
  pageDump->AddScalar("dead_count", "objects", deadCount);
  pageDump->AddScalar("free_count", "objects", freeCount);
  pageDump->AddScalar("live_size", "bytes", liveSize);
  pageDump->AddScalar("dead_size", "bytes", deadSize);
  pageDump->AddScalar("free_size", "bytes", freeSize);
  heapInfo.freeSize += freeSize;
  heapInfo.freeCount += freeCount;
}
