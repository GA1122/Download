void LargeObjectPage::poisonUnmarkedObjects() {
  HeapObjectHeader* header = heapObjectHeader();
  if (!header->isMarked())
    ASAN_POISON_MEMORY_REGION(header->payload(), header->payloadSize());
}
