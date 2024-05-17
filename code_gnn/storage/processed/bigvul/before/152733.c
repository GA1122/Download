Histogram::Histogram(const std::string& name,
                     Sample minimum,
                     Sample maximum,
                     const BucketRanges* ranges)
  : HistogramBase(name),
    bucket_ranges_(ranges),
    declared_min_(minimum),
    declared_max_(maximum) {
  if (ranges)
    samples_.reset(new SampleVector(HashMetricName(name), ranges));
}
