bool Histogram::HasConstructionArguments(Sample expected_minimum,
                                         Sample expected_maximum,
                                         uint32_t expected_bucket_count) const {
  return ((expected_minimum == declared_min_) &&
          (expected_maximum == declared_max_) &&
          (expected_bucket_count == bucket_count()));
}
