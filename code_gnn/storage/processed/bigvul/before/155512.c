void DataReductionProxySettings::RecordStartupSavings() const {
  const unsigned int kMinOriginalContentLengthBytes =
      10 * 1024 * 1024;   

  if (!IsDataReductionProxyEnabled())
    return;

  DCHECK(data_reduction_proxy_service_->compression_stats());
  int64_t original_content_length =
      data_reduction_proxy_service_->compression_stats()
          ->GetHttpOriginalContentLength();
  int64_t received_content_length =
      data_reduction_proxy_service_->compression_stats()
          ->GetHttpReceivedContentLength();
  if (original_content_length < kMinOriginalContentLengthBytes)
    return;
  int savings_percent =
      static_cast<int>(((original_content_length - received_content_length) /
                        (float)original_content_length) *
                       100.0);
  if (savings_percent >= 0) {
    UMA_HISTOGRAM_PERCENTAGE("DataReductionProxy.StartupSavingsPercent",
                             savings_percent > 0 ? savings_percent : 0);
  }
  if (savings_percent < 0) {
    UMA_HISTOGRAM_PERCENTAGE("DataReductionProxy.StartupNegativeSavingsPercent",
                             -savings_percent);
  }
}
