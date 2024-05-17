void NormalPageArena::UpdateRemainingAllocationSize() {
  if (last_remaining_allocation_size_ > RemainingAllocationSize()) {
    GetThreadState()->Heap().HeapStats().IncreaseAllocatedObjectSize(
        last_remaining_allocation_size_ - RemainingAllocationSize());
    last_remaining_allocation_size_ = RemainingAllocationSize();
  }
  DCHECK_EQ(last_remaining_allocation_size_, RemainingAllocationSize());
}
