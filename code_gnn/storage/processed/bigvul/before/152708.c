HistogramBase* BooleanHistogram::FactoryGet(const std::string& name,
                                            int32_t flags) {
  return Factory(name, flags).Build();
}
