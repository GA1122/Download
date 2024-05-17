HistogramBase* LinearHistogram::FactoryTimeGet(const std::string& name,
                                               TimeDelta minimum,
                                               TimeDelta maximum,
                                               uint32_t bucket_count,
                                               int32_t flags) {
  return FactoryGet(name, static_cast<Sample>(minimum.InMilliseconds()),
                    static_cast<Sample>(maximum.InMilliseconds()), bucket_count,
                    flags);
}
