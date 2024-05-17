void LinearHistogram::InitializeBucketRanges(Sample minimum,
                                             Sample maximum,
                                             BucketRanges* ranges) {
  double min = minimum;
  double max = maximum;
  size_t bucket_count = ranges->bucket_count();
  for (size_t i = 1; i < bucket_count; ++i) {
    double linear_range =
        (min * (bucket_count - 1 - i) + max * (i - 1)) / (bucket_count - 2);
    ranges->set_range(i, static_cast<Sample>(linear_range + 0.5));
    base::debug::Alias(&linear_range);
  }
  ranges->set_range(ranges->bucket_count(), HistogramBase::kSampleType_MAX);
  ranges->ResetChecksum();
}
