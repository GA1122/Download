bool LinearHistogram::PrintEmptyBucket(uint32_t index) const {
  return bucket_description_.find(ranges(index)) == bucket_description_.end();
}
