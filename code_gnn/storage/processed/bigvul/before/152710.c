HistogramBase* CustomHistogram::FactoryGet(
    const std::string& name,
    const std::vector<Sample>& custom_ranges,
    int32_t flags) {
  CHECK(ValidateCustomRanges(custom_ranges));

  return Factory(name, &custom_ranges, flags).Build();
}
