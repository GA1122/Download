void PersistentHistogramAllocator::FinalizeHistogram(Reference ref,
                                                     bool registered) {
  if (registered) {
    memory_allocator_->MakeIterable(ref);
  } else {
    memory_allocator_->ChangeType(ref, 0,
                                  PersistentHistogramData::kPersistentTypeId,
                                   false);
  }
}
