uint32_t BpMemoryHeap::getFlags() const {
    assertMapped();
 return mFlags;
}
