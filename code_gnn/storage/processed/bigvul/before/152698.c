HistogramBase* CustomHistogram::DeserializeInfoImpl(PickleIterator* iter) {
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

  std::vector<Sample> sample_ranges(bucket_count - 1);

  for (uint32_t i = 0; i < sample_ranges.size(); ++i) {
    if (!iter->ReadInt(&sample_ranges[i]))
      return NULL;
  }

  HistogramBase* histogram = CustomHistogram::FactoryGet(
      histogram_name, sample_ranges, flags);
  if (!ValidateRangeChecksum(*histogram, range_checksum)) {
    return NULL;
  }
  return histogram;
}
