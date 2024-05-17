  Histogram* CreateHistogram(const char* name,
                             HistogramBase::Sample min,
                             HistogramBase::Sample max,
                             size_t bucket_count) {
    BucketRanges* ranges = new BucketRanges(bucket_count + 1);
    Histogram::InitializeBucketRanges(min, max, ranges);
    const BucketRanges* registered_ranges =
        StatisticsRecorder::RegisterOrDeleteDuplicateRanges(ranges);
    return new Histogram(name, min, max, registered_ranges);
  }
