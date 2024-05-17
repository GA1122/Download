std::vector<Sample> CustomHistogram::ArrayToCustomRanges(
    const Sample* values, uint32_t num_values) {
  std::vector<Sample> all_values;
  for (uint32_t i = 0; i < num_values; ++i) {
    Sample value = values[i];
    all_values.push_back(value);

    all_values.push_back(value + 1);
  }
  return all_values;
}
