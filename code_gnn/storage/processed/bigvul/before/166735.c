bool NormalPageArena::ExpandObject(HeapObjectHeader* header, size_t new_size) {
  if (header->PayloadSize() >= new_size)
    return true;
  size_t allocation_size = ThreadHeap::AllocationSizeFromSize(new_size);
  DCHECK_GT(allocation_size, header->size());
  size_t expand_size = allocation_size - header->size();
  if (IsObjectAllocatedAtAllocationPoint(header) &&
      expand_size <= remaining_allocation_size_) {
    current_allocation_point_ += expand_size;
    DCHECK_GE(remaining_allocation_size_, expand_size);
    SetRemainingAllocationSize(remaining_allocation_size_ - expand_size);
    SET_MEMORY_ACCESSIBLE(header->PayloadEnd(), expand_size);
    header->SetSize(allocation_size);
#if DCHECK_IS_ON()
    DCHECK(FindPageFromAddress(header->PayloadEnd() - 1));
#endif
    return true;
  }
  return false;
}
