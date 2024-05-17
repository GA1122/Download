void* BpMemoryHeap::getBase() const {
    assertMapped();
 return mBase;
}
