  Factory(const std::string& name,
          const std::vector<Sample>* custom_ranges,
          int32_t flags)
    : Histogram::Factory(name, CUSTOM_HISTOGRAM, 0, 0, 0, flags) {
    custom_ranges_ = custom_ranges;
  }
