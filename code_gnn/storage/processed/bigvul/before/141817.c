uint64_t MetricsLog::Hash(const std::string& value) {
  uint64_t hash = base::HashMetricName(value);

  DVLOG(1) << "Metrics: Hash numeric [" << value << "]=[" << hash << "]";

  return hash;
}
