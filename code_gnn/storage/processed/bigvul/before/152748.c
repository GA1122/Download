bool CustomHistogram::SerializeInfoImpl(Pickle* pickle) const {
  if (!Histogram::SerializeInfoImpl(pickle))
    return false;

  for (uint32_t i = 1; i < bucket_ranges()->bucket_count(); ++i) {
    if (!pickle->WriteInt(bucket_ranges()->range(i)))
      return false;
  }
  return true;
}
