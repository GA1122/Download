void ThreadHeapStats::IncreaseAllocatedSpace(size_t delta) {
  allocated_space_ += delta;
  ProcessHeap::IncreaseTotalAllocatedSpace(delta);
}
