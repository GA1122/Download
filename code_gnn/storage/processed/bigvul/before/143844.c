  CreateMetricsFileWithHistograms(int histogram_count) {
    return CreateMetricsFileWithHistograms(
        metrics_file(), base::Time::Now(), histogram_count,
        [](base::PersistentHistogramAllocator* allocator) {});
  }
