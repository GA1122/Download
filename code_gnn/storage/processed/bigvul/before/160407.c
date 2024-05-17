void LargeObjectPage::takeSnapshot(
    base::trace_event::MemoryAllocatorDump* pageDump,
    ThreadState::GCSnapshotInfo& info,
    HeapSnapshotInfo&) {
  size_t liveSize = 0;
  size_t deadSize = 0;
  size_t liveCount = 0;
  size_t deadCount = 0;
  HeapObjectHeader* header = heapObjectHeader();
  size_t gcInfoIndex = header->gcInfoIndex();
  size_t payloadSize = header->payloadSize();
  if (header->isMarked()) {
    liveCount = 1;
    liveSize += payloadSize;
    info.liveCount[gcInfoIndex]++;
    info.liveSize[gcInfoIndex] += payloadSize;
  } else {
    deadCount = 1;
    deadSize += payloadSize;
    info.deadCount[gcInfoIndex]++;
    info.deadSize[gcInfoIndex] += payloadSize;
  }

  pageDump->AddScalar("live_count", "objects", liveCount);
  pageDump->AddScalar("dead_count", "objects", deadCount);
  pageDump->AddScalar("live_size", "bytes", liveSize);
  pageDump->AddScalar("dead_size", "bytes", deadSize);
}
