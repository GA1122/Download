Histogram::Histogram(const std::string& name,
                     Sample minimum,
                     Sample maximum,
                     const BucketRanges* ranges,
                     HistogramBase::AtomicCount* counts,
                     HistogramBase::AtomicCount* logged_counts,
                     uint32_t counts_size,
                     HistogramSamples::Metadata* meta,
                     HistogramSamples::Metadata* logged_meta)
  : HistogramBase(name),
    bucket_ranges_(ranges),
    declared_min_(minimum),
    declared_max_(maximum) {
  if (ranges) {
    samples_.reset(new SampleVector(HashMetricName(name),
                                    counts, counts_size, meta, ranges));
    logged_samples_.reset(new SampleVector(samples_->id(), logged_counts,
                                           counts_size, logged_meta, ranges));
  }
}
