  virtual BucketRanges* CreateRanges() {
    BucketRanges* ranges = new BucketRanges(bucket_count_ + 1);
    Histogram::InitializeBucketRanges(minimum_, maximum_, ranges);
    return ranges;
  }
