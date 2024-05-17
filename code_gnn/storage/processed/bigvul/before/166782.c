void BaseArena::TakeSnapshot(const String& dump_base_name,
                             ThreadState::GCSnapshotInfo& info) {
  base::trace_event::MemoryAllocatorDump* allocator_dump =
      BlinkGCMemoryDumpProvider::Instance()
          ->CreateMemoryAllocatorDumpForCurrentGC(dump_base_name);
  size_t page_count = 0;
  BasePage::HeapSnapshotInfo heap_info;
  for (BasePage* page = first_unswept_page_; page; page = page->Next()) {
    String dump_name = dump_base_name +
                       String::Format("/pages/page_%lu",
                                      static_cast<unsigned long>(page_count++));
    base::trace_event::MemoryAllocatorDump* page_dump =
        BlinkGCMemoryDumpProvider::Instance()
            ->CreateMemoryAllocatorDumpForCurrentGC(dump_name);

    page->TakeSnapshot(page_dump, info, heap_info);
  }
  allocator_dump->AddScalar("blink_page_count", "objects", page_count);

  allocator_dump->AddScalar("free_size", "bytes", heap_info.free_size);
  allocator_dump->AddScalar("free_count", "objects", heap_info.free_count);
}
