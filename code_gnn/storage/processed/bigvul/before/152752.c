bool CustomHistogram::ValidateCustomRanges(
    const std::vector<Sample>& custom_ranges) {
  bool has_valid_range = false;
  for (uint32_t i = 0; i < custom_ranges.size(); i++) {
    Sample sample = custom_ranges[i];
    if (sample < 0 || sample > HistogramBase::kSampleType_MAX - 1)
      return false;
    if (sample != 0)
      has_valid_range = true;
  }
  return has_valid_range;
}
