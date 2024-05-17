void RecordParallelDownloadAddStreamSuccess(bool success) {
  UMA_HISTOGRAM_BOOLEAN("Download.ParallelDownloadAddStreamSuccess", success);
}
