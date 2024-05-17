void AssertObjectHasGCInfo(const void* payload, size_t gc_info_index) {
  HeapObjectHeader::CheckFromPayload(payload);
#if !defined(COMPONENT_BUILD)
  DCHECK_EQ(HeapObjectHeader::FromPayload(payload)->GcInfoIndex(),
            gc_info_index);
#endif
}
