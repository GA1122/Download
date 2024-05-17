void NormalPageArena::PromptlyFreeObject(HeapObjectHeader* header) {
  DCHECK(!GetThreadState()->SweepForbidden());
  Address address = reinterpret_cast<Address>(header);
  Address payload = header->Payload();
  size_t size = header->size();
  size_t payload_size = header->PayloadSize();
  DCHECK_GT(size, 0u);
#if DCHECK_IS_ON()
  DCHECK_EQ(PageFromObject(address), FindPageFromAddress(address));
#endif
  {
    ThreadState::SweepForbiddenScope forbidden_scope(GetThreadState());
    header->Finalize(payload, payload_size);
    if (IsObjectAllocatedAtAllocationPoint(header)) {
      current_allocation_point_ -= size;
      DCHECK_EQ(address, current_allocation_point_);
      SetRemainingAllocationSize(remaining_allocation_size_ + size);
      SET_MEMORY_INACCESSIBLE(address, size);
      reinterpret_cast<NormalPage*>(PageFromObject(header))
          ->object_start_bit_map()
          ->ClearBit(address);
      return;
    }
    PromptlyFreeObjectInFreeList(header, size);
  }
}
