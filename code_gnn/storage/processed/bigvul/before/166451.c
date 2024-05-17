void RecordParallelizableDownloadAverageStats(
    int64_t bytes_downloaded,
    const base::TimeDelta& time_span) {
  if (time_span.is_zero() || bytes_downloaded <= 0)
    return;

  int64_t average_bandwidth =
      CalculateBandwidthBytesPerSecond(bytes_downloaded, time_span);
  int64_t file_size_kb = bytes_downloaded / 1024;
  RecordBandwidthMetric("Download.ParallelizableDownloadBandwidth",
                        average_bandwidth);
  UMA_HISTOGRAM_LONG_TIMES("Download.Parallelizable.DownloadTime", time_span);
  UMA_HISTOGRAM_CUSTOM_COUNTS("Download.Parallelizable.FileSize", file_size_kb,
                              1, kMaxFileSizeKb, 50);
  if (average_bandwidth > kHighBandwidthBytesPerSecond) {
    UMA_HISTOGRAM_LONG_TIMES(
        "Download.Parallelizable.DownloadTime.HighDownloadBandwidth",
        time_span);
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "Download.Parallelizable.FileSize.HighDownloadBandwidth", file_size_kb,
        1, kMaxFileSizeKb, 50);
  }
}
