double Histogram::GetPeakBucketSize(const SampleVector& samples) const {
  double max = 0;
  for (uint32_t i = 0; i < bucket_count() ; ++i) {
    double current_size = GetBucketSize(samples.GetCountAtIndex(i), i);
    if (current_size > max)
      max = current_size;
  }
  return max;
}
