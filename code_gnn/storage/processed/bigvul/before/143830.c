  TestHistogramProvider(std::unique_ptr<PersistentHistogramAllocator> allocator)
      : allocator_(std::move(allocator)), weak_factory_(this) {
    StatisticsRecorder::RegisterHistogramProvider(weak_factory_.GetWeakPtr());
  }
