std::unique_ptr<HistogramBase> CustomHistogram::PersistentCreate(
    const std::string& name,
    const BucketRanges* ranges,
    HistogramBase::AtomicCount* counts,
    HistogramBase::AtomicCount* logged_counts,
    uint32_t counts_size,
    HistogramSamples::Metadata* meta,
    HistogramSamples::Metadata* logged_meta) {
  return WrapUnique(new CustomHistogram(
      name, ranges, counts, logged_counts, counts_size, meta, logged_meta));
}
