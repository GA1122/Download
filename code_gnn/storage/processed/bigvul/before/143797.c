PersistentHistogramAllocator::Iterator::GetNextWithIgnore(Reference ignore) {
  PersistentMemoryAllocator::Reference ref;
  while ((ref = memory_iter_.GetNextOfType<PersistentHistogramData>()) != 0) {
    if (ref != ignore)
      return allocator_->GetHistogram(ref);
  }
  return nullptr;
}
