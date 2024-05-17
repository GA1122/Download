HistogramBase* LinearHistogram::FactoryGet(const std::string& name,
                                           Sample minimum,
                                           Sample maximum,
                                           uint32_t bucket_count,
                                           int32_t flags) {
  return FactoryGetWithRangeDescription(
      name, minimum, maximum, bucket_count, flags, NULL);
}
