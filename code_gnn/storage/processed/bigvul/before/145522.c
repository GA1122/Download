void RecordCertificateHistograms(const net::SSLInfo& ssl_info,
                                 ResourceType resource_type) {
  static const int64_t kJanuary2017 = INT64_C(13127702400000000);
  static const int64_t kJune2016 = INT64_C(13109213000000000);
  static const int64_t kJanuary2016 = INT64_C(13096080000000000);

  SHA1HistogramTypes sha1_histogram = SHA1_NOT_PRESENT;
  if (ssl_info.cert_status & net::CERT_STATUS_SHA1_SIGNATURE_PRESENT) {
    DCHECK(ssl_info.cert.get());
    if (ssl_info.cert->valid_expiry() >=
        base::Time::FromInternalValue(kJanuary2017)) {
      sha1_histogram = SHA1_EXPIRES_AFTER_JANUARY_2017;
    } else if (ssl_info.cert->valid_expiry() >=
               base::Time::FromInternalValue(kJune2016)) {
      sha1_histogram = SHA1_EXPIRES_AFTER_JUNE_2016;
    } else if (ssl_info.cert->valid_expiry() >=
               base::Time::FromInternalValue(kJanuary2016)) {
      sha1_histogram = SHA1_EXPIRES_AFTER_JANUARY_2016;
    } else {
      sha1_histogram = SHA1_PRESENT;
    }
  }
  if (resource_type == RESOURCE_TYPE_MAIN_FRAME) {
    UMA_HISTOGRAM_ENUMERATION("Net.Certificate.SHA1.MainFrame",
                              sha1_histogram,
                              SHA1_HISTOGRAM_TYPES_MAX);
  } else {
    UMA_HISTOGRAM_ENUMERATION("Net.Certificate.SHA1.Subresource",
                              sha1_histogram,
                              SHA1_HISTOGRAM_TYPES_MAX);
  }
}
