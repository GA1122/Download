void HeapObjectHeader::finalize(Address object, size_t objectSize) {
  HeapAllocHooks::freeHookIfEnabled(object);
  const GCInfo* gcInfo = ThreadHeap::gcInfo(gcInfoIndex());
  if (gcInfo->hasFinalizer())
    gcInfo->m_finalize(object);

  ASAN_RETIRE_CONTAINER_ANNOTATION(object, objectSize);
}
