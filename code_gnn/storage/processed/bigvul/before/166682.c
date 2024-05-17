void ThreadHeapStats::DecreaseAllocatedObjectSize(size_t delta) {
  allocated_object_size_ -= delta;
  ProcessHeap::DecreaseTotalAllocatedObjectSize(delta);
}
