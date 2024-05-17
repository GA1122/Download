bool ValidateRangeChecksum(const HistogramBase& histogram,
                           uint32_t range_checksum) {
  const Histogram& casted_histogram =
      static_cast<const Histogram&>(histogram);

  return casted_histogram.bucket_ranges()->checksum() == range_checksum;
}
