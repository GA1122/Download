void RecordDownloadCompleted(int64_t download_len,
                             bool is_parallelizable,
                             DownloadSource download_source) {
  RecordDownloadCountWithSource(COMPLETED_COUNT, download_source);
  int64_t max = 1024 * 1024 * 1024;   
  download_len /= 1024;               
  UMA_HISTOGRAM_CUSTOM_COUNTS("Download.DownloadSize", download_len, 1, max,
                              256);
  if (is_parallelizable) {
    UMA_HISTOGRAM_CUSTOM_COUNTS("Download.DownloadSize.Parallelizable",
                                download_len, 1, max, 256);
  }
}
