uint32_t BpMemoryHeap::getOffset() const {
    assertMapped();
 return mOffset;
}
