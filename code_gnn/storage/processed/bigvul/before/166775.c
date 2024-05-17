void NormalPageArena::SetRemainingAllocationSize(
    size_t new_remaining_allocation_size) {
  remaining_allocation_size_ = new_remaining_allocation_size;

  if (last_remaining_allocation_size_ > remaining_allocation_size_)
    GetThreadState()->Heap().HeapStats().IncreaseAllocatedObjectSize(
        last_remaining_allocation_size_ - remaining_allocation_size_);
  else if (last_remaining_allocation_size_ != remaining_allocation_size_)
    GetThreadState()->Heap().HeapStats().DecreaseAllocatedObjectSize(
        remaining_allocation_size_ - last_remaining_allocation_size_);
  last_remaining_allocation_size_ = remaining_allocation_size_;
}
