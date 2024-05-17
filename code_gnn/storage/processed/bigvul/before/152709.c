HistogramBase* BooleanHistogram::FactoryGet(const char* name, int32_t flags) {
  return FactoryGet(std::string(name), flags);
}
