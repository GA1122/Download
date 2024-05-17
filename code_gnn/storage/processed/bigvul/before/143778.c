void PersistentHistogramAllocator::ClearLastCreatedReferenceForTesting() {
   subtle::NoBarrier_Store(&last_created_, 0);
 }
