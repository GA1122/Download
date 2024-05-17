  void ChromeNetworkDelegate::AccumulateContentLength(
      int64 received_content_length, int64 original_content_length,
    bool via_data_reduction_proxy) {
//     chrome_browser_net::DataReductionRequestType data_reduction_type) {
    DCHECK_GE(received_content_length, 0);
    DCHECK_GE(original_content_length, 0);
    StoreAccumulatedContentLength(received_content_length,
                                  original_content_length,
                                via_data_reduction_proxy);
//                                 data_reduction_type);
    received_content_length_ += received_content_length;
    original_content_length_ += original_content_length;
  }