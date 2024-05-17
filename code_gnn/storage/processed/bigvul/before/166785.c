void LargeObjectPage::TakeSnapshot(
    base::trace_event::MemoryAllocatorDump* page_dump,
    ThreadState::GCSnapshotInfo& info,
    HeapSnapshotInfo&) {
  size_t live_size = 0;
  size_t dead_size = 0;
  size_t live_count = 0;
  size_t dead_count = 0;
  HeapObjectHeader* header = GetHeapObjectHeader();
  size_t gc_info_index = header->GcInfoIndex();
  size_t payload_size = header->PayloadSize();
  if (header->IsMarked()) {
    live_count = 1;
    live_size += payload_size;
    info.live_count[gc_info_index]++;
    info.live_size[gc_info_index] += payload_size;
  } else {
    dead_count = 1;
    dead_size += payload_size;
    info.dead_count[gc_info_index]++;
    info.dead_size[gc_info_index] += payload_size;
  }

  page_dump->AddScalar("live_count", "objects", live_count);
  page_dump->AddScalar("dead_count", "objects", dead_count);
  page_dump->AddScalar("live_size", "bytes", live_size);
  page_dump->AddScalar("dead_size", "bytes", dead_size);
}
