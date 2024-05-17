void RecordParallelizableContentLength(int64_t content_length) {
  UMA_HISTOGRAM_CUSTOM_COUNTS("Download.ContentLength.Parallelizable",
                              content_length / 1024, 1, kMaxFileSizeKb, 50);
}
