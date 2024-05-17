uint32_t Histogram::bucket_count() const {
  return static_cast<uint32_t>(bucket_ranges_->bucket_count());
}
