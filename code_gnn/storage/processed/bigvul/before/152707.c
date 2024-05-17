HistogramBase* LinearHistogram::FactoryGet(const char* name,
                                           Sample minimum,
                                           Sample maximum,
                                           uint32_t bucket_count,
                                           int32_t flags) {
  return FactoryGet(std::string(name), minimum, maximum, bucket_count, flags);
}
