  Factory(const std::string& name,
          HistogramBase::Sample minimum,
          HistogramBase::Sample maximum,
          uint32_t bucket_count,
          int32_t flags)
    : Factory(name, HISTOGRAM, minimum, maximum, bucket_count, flags) {}
