double LinearHistogram::GetBucketSize(Count current, uint32_t i) const {
  DCHECK_GT(ranges(i + 1), ranges(i));
  double denominator = ranges(i + 1) - ranges(i);
  return current/denominator;
}
