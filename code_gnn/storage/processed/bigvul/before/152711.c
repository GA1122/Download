HistogramBase* CustomHistogram::FactoryGet(
    const char* name,
    const std::vector<Sample>& custom_ranges,
    int32_t flags) {
  return FactoryGet(std::string(name), custom_ranges, flags);
}
