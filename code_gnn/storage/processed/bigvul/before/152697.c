HistogramBase* BooleanHistogram::DeserializeInfoImpl(PickleIterator* iter) {
  std::string histogram_name;
  int flags;
  int declared_min;
  int declared_max;
  uint32_t bucket_count;
  uint32_t range_checksum;

  if (!ReadHistogramArguments(iter, &histogram_name, &flags, &declared_min,
                              &declared_max, &bucket_count, &range_checksum)) {
    return NULL;
  }

  HistogramBase* histogram = BooleanHistogram::FactoryGet(
      histogram_name, flags);
  if (!ValidateRangeChecksum(*histogram, range_checksum)) {
    return NULL;
  }
  return histogram;
}
