void NormalPage::poisonUnmarkedObjects() {
  for (Address headerAddress = payload(); headerAddress < payloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(headerAddress);
    ASSERT(header->size() < blinkPagePayloadSize());
    if (header->isFree()) {
      headerAddress += header->size();
      continue;
    }
    if (!header->isMarked())
      ASAN_POISON_MEMORY_REGION(header->payload(), header->payloadSize());
    headerAddress += header->size();
  }
}
