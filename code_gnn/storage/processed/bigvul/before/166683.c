void ThreadHeapStats::DecreaseAllocatedSpace(size_t delta) {
  allocated_space_ -= delta;
  ProcessHeap::DecreaseTotalAllocatedSpace(delta);
}
