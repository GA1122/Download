void RecordBandwidthMetric(const std::string& metric, int bandwidth) {
  base::UmaHistogramCustomCounts(metric, bandwidth, 1, 50 * 1000 * 1000, 50);
}
