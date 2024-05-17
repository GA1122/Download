void RecordParallelDownloadRequestCount(int request_count) {
  UMA_HISTOGRAM_CUSTOM_COUNTS("Download.ParallelDownloadRequestCount",
                              request_count, 1, 10, 11);
}
