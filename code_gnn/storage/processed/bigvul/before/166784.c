void NormalPage::TakeSnapshot(base::trace_event::MemoryAllocatorDump* page_dump,
                              ThreadState::GCSnapshotInfo& info,
                              HeapSnapshotInfo& heap_info) {
  HeapObjectHeader* header = nullptr;
  size_t live_count = 0;
  size_t dead_count = 0;
  size_t free_count = 0;
  size_t live_size = 0;
  size_t dead_size = 0;
  size_t free_size = 0;
  for (Address header_address = Payload(); header_address < PayloadEnd();
       header_address += header->size()) {
    header = reinterpret_cast<HeapObjectHeader*>(header_address);
    if (header->IsFree()) {
      free_count++;
      free_size += header->size();
    } else if (header->IsMarked()) {
      live_count++;
      live_size += header->size();

      size_t gc_info_index = header->GcInfoIndex();
      info.live_count[gc_info_index]++;
      info.live_size[gc_info_index] += header->size();
    } else {
      dead_count++;
      dead_size += header->size();

      size_t gc_info_index = header->GcInfoIndex();
      info.dead_count[gc_info_index]++;
      info.dead_size[gc_info_index] += header->size();
    }
  }

  page_dump->AddScalar("live_count", "objects", live_count);
  page_dump->AddScalar("dead_count", "objects", dead_count);
  page_dump->AddScalar("free_count", "objects", free_count);
  page_dump->AddScalar("live_size", "bytes", live_size);
  page_dump->AddScalar("dead_size", "bytes", dead_size);
  page_dump->AddScalar("free_size", "bytes", free_size);
  heap_info.free_size += free_size;
  heap_info.free_count += free_count;
}
