void NormalPageArena::PromptlyFreeObjectInFreeList(HeapObjectHeader* header,
                                                   size_t size) {
  Address address = reinterpret_cast<Address>(header);
  NormalPage* page = reinterpret_cast<NormalPage*>(PageFromObject(header));
  if (page->HasBeenSwept()) {
    Address payload = header->Payload();
    size_t payload_size = header->PayloadSize();
    SET_MEMORY_INACCESSIBLE(payload, payload_size);
    CHECK_MEMORY_INACCESSIBLE(payload, payload_size);
    AddToFreeList(address, size);
    promptly_freed_size_ += size;
  } else {
    header->Unmark();
  }
  GetThreadState()->Heap().HeapStats().DecreaseAllocatedObjectSize(size);
}
