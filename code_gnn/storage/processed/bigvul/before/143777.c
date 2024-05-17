size_t CalculateRequiredCountsBytes(size_t bucket_count) {
  const size_t kBytesPerBucket = 2 * sizeof(HistogramBase::AtomicCount);

  if (bucket_count > std::numeric_limits<size_t>::max() / kBytesPerBucket)
    return 0;

  return bucket_count * kBytesPerBucket;
}
