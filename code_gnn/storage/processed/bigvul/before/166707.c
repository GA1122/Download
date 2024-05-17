void ThreadHeap::ReportMemoryUsageHistogram() {
  static size_t supported_max_size_in_mb = 4 * 1024;
  static size_t observed_max_size_in_mb = 0;

  if (!IsMainThread())
    return;
  size_t size_in_mb =
      ThreadState::Current()->Heap().HeapStats().AllocatedSpace() / 1024 /
          1024 +
      1;
  if (size_in_mb >= supported_max_size_in_mb)
    size_in_mb = supported_max_size_in_mb - 1;
  if (size_in_mb > observed_max_size_in_mb) {
    DEFINE_THREAD_SAFE_STATIC_LOCAL(
        EnumerationHistogram, commited_size_histogram,
        ("BlinkGC.CommittedSize", supported_max_size_in_mb));
    commited_size_histogram.Count(size_in_mb);
    observed_max_size_in_mb = size_in_mb;
  }
}
