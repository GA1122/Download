  void RecordSCTHistogramIfNeeded(
      const base::Optional<net::SSLInfo>& ssl_info) {
    if (is_main_frame_ && url_.SchemeIsCryptographic() &&
        ssl_info.has_value()) {
      int num_valid_scts = 0;
      for (const auto& signed_certificate_timestamps :
           ssl_info->signed_certificate_timestamps) {
        if (signed_certificate_timestamps.status == net::ct::SCT_STATUS_OK)
          ++num_valid_scts;
      }
      UMA_HISTOGRAM_COUNTS_100(
          "Net.CertificateTransparency.MainFrameValidSCTCount", num_valid_scts);
    }
  }
