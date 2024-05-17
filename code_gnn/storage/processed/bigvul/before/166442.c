void RecordFileBandwidth(size_t length,
                         base::TimeDelta elapsed_time) {
  RecordBandwidthMetric("Download.BandwidthOverallBytesPerSecond",
                        CalculateBandwidthBytesPerSecond(length, elapsed_time));
}
