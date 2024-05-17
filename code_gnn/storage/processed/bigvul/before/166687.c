void ThreadHeapStats::IncreaseAllocatedObjectSize(size_t delta) {
  allocated_object_size_ += delta;
  ProcessHeap::IncreaseTotalAllocatedObjectSize(delta);
}
