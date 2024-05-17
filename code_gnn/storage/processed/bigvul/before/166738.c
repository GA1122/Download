void LargeObjectArena::FreeLargeObjectPage(LargeObjectPage* object) {
  ASAN_UNPOISON_MEMORY_REGION(object->Payload(), object->PayloadSize());
  object->GetHeapObjectHeader()->Finalize(object->Payload(),
                                          object->PayloadSize());
  GetThreadState()->Heap().HeapStats().DecreaseAllocatedSpace(object->size());

  ASAN_UNPOISON_MEMORY_REGION(object->GetHeapObjectHeader(),
                              sizeof(HeapObjectHeader));
  ASAN_UNPOISON_MEMORY_REGION(object->GetAddress() + object->size(),
                              kAllocationGranularity);

  PageMemory* memory = object->Storage();
  object->~LargeObjectPage();
  delete memory;
}
