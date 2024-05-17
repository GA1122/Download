void LargeObjectPage::PoisonUnmarkedObjects() {
  HeapObjectHeader* header = GetHeapObjectHeader();
  if (!header->IsMarked())
    ASAN_POISON_MEMORY_REGION(header->Payload(), header->PayloadSize());
}
