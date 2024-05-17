std::unique_ptr<MetricsLog> MetricsLog::IndependentMetricsLoader::ReleaseLog() {
  return std::move(log_);
}
