int BpMemoryHeap::getHeapID() const {
    assertMapped();
 return mHeapId;
}
