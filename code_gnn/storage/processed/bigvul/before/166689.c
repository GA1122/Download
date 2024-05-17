void ThreadHeapStats::IncreaseMarkedObjectSize(size_t delta) {
  marked_object_size_ += delta;
  ProcessHeap::IncreaseTotalMarkedObjectSize(delta);
}
