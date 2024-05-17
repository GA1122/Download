void RecordDownloadInterrupted(DownloadInterruptReason reason,
                               int64_t received,
                               int64_t total,
                               bool is_parallelizable,
                               bool is_parallel_download_enabled,
                               DownloadSource download_source) {
  RecordDownloadCountWithSource(INTERRUPTED_COUNT, download_source);
  if (is_parallelizable) {
    RecordParallelizableDownloadCount(INTERRUPTED_COUNT,
                                      is_parallel_download_enabled);
  }

  std::vector<base::HistogramBase::Sample> samples =
      base::CustomHistogram::ArrayToCustomEnumRanges(kAllInterruptReasonCodes);
  UMA_HISTOGRAM_CUSTOM_ENUMERATION("Download.InterruptedReason", reason,
                                   samples);

  std::string name = CreateHistogramNameWithSuffix("Download.InterruptedReason",
                                                   download_source);
  base::HistogramBase* counter = base::CustomHistogram::FactoryGet(
      name, samples, base::HistogramBase::kUmaTargetedHistogramFlag);
  counter->Add(reason);

  if (is_parallel_download_enabled) {
    UMA_HISTOGRAM_CUSTOM_ENUMERATION(
        "Download.InterruptedReason.ParallelDownload", reason, samples);
  }

  static const int kBuckets = 30;
  static const int64_t kMaxKb = 1 << kBuckets;   
  int64_t delta_bytes = total - received;
  bool unknown_size = total <= 0;
  int64_t received_kb = received / 1024;
  int64_t total_kb = total / 1024;
  UMA_HISTOGRAM_CUSTOM_COUNTS("Download.InterruptedReceivedSizeK", received_kb,
                              1, kMaxKb, kBuckets);
  if (is_parallel_download_enabled) {
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "Download.InterruptedReceivedSizeK.ParallelDownload", received_kb, 1,
        kMaxKb, kBuckets);
  }

  if (!unknown_size) {
    UMA_HISTOGRAM_CUSTOM_COUNTS("Download.InterruptedTotalSizeK", total_kb, 1,
                                kMaxKb, kBuckets);
    if (is_parallel_download_enabled) {
      UMA_HISTOGRAM_CUSTOM_COUNTS(
          "Download.InterruptedTotalSizeK.ParallelDownload", total_kb, 1,
          kMaxKb, kBuckets);
    }
    if (delta_bytes == 0) {
      RecordDownloadCountWithSource(INTERRUPTED_AT_END_COUNT, download_source);
      UMA_HISTOGRAM_CUSTOM_ENUMERATION("Download.InterruptedAtEndReason",
                                       reason, samples);

      if (is_parallelizable) {
        RecordParallelizableDownloadCount(INTERRUPTED_AT_END_COUNT,
                                          is_parallel_download_enabled);
        UMA_HISTOGRAM_CUSTOM_ENUMERATION(
            "Download.InterruptedAtEndReason.ParallelDownload", reason,
            samples);
      }
    } else if (delta_bytes > 0) {
      UMA_HISTOGRAM_CUSTOM_COUNTS("Download.InterruptedOverrunBytes",
                                  delta_bytes, 1, kMaxKb, kBuckets);
      if (is_parallel_download_enabled) {
        UMA_HISTOGRAM_CUSTOM_COUNTS(
            "Download.InterruptedOverrunBytes.ParallelDownload", delta_bytes, 1,
            kMaxKb, kBuckets);
      }
    } else {
      UMA_HISTOGRAM_CUSTOM_COUNTS("Download.InterruptedUnderrunBytes",
                                  -delta_bytes, 1, kMaxKb, kBuckets);
      if (is_parallel_download_enabled) {
        UMA_HISTOGRAM_CUSTOM_COUNTS(
            "Download.InterruptedUnderrunBytes.ParallelDownload", -delta_bytes,
            1, kMaxKb, kBuckets);
      }
    }
  }
}
