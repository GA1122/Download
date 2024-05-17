void NormalPage::PoisonUnmarkedObjects() {
  for (Address header_address = Payload(); header_address < PayloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    DCHECK_LT(header->size(), BlinkPagePayloadSize());
    if (header->IsFree()) {
      header_address += header->size();
      continue;
    }
    if (!header->IsMarked())
      ASAN_POISON_MEMORY_REGION(header->Payload(), header->PayloadSize());
    header_address += header->size();
  }
}
