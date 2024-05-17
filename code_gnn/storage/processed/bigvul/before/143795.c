GlobalHistogramAllocator* GlobalHistogramAllocator::Get() {
  return reinterpret_cast<GlobalHistogramAllocator*>(
      subtle::Acquire_Load(&g_histogram_allocator));
}
