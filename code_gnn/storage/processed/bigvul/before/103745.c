static void* CreateHistogram(
    const char *name, int min, int max, size_t buckets) {
  if (min <= 0)
    min = 1;
  base::Histogram* histogram = base::Histogram::FactoryGet(
      name, min, max, buckets, base::Histogram::kUmaTargetedHistogramFlag);
  return histogram;
}
