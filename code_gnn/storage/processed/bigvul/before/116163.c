void ResourceDispatcherHostImpl::ResponseCompleted(net::URLRequest* request) {
  VLOG(1) << "ResponseCompleted: " << request->url().spec();
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  if (info->GetResourceType() == ResourceType::MAIN_FRAME) {
    UMA_HISTOGRAM_CUSTOM_ENUMERATION(
        "Net.ErrorCodesForMainFrame3",
        -request->status().error(),
        base::CustomHistogram::ArrayToCustomRanges(
            kAllNetErrorCodes, arraysize(kAllNetErrorCodes)));

    if (request->url().SchemeIsSecure() &&
        request->url().host() == "www.google.com") {
      UMA_HISTOGRAM_CUSTOM_ENUMERATION(
          "Net.ErrorCodesForHTTPSGoogleMainFrame2",
          -request->status().error(),
          base::CustomHistogram::ArrayToCustomRanges(
              kAllNetErrorCodes, arraysize(kAllNetErrorCodes)));
    }
  } else {
    UMA_HISTOGRAM_CUSTOM_ENUMERATION(
        "Net.ErrorCodesForSubresources2",
        -request->status().error(),
        base::CustomHistogram::ArrayToCustomRanges(
            kAllNetErrorCodes, arraysize(kAllNetErrorCodes)));
  }

  std::string security_info;
  const net::SSLInfo& ssl_info = request->ssl_info();
  if (ssl_info.cert != NULL) {
    int cert_id = CertStore::GetInstance()->StoreCert(ssl_info.cert,
                                                      info->GetChildID());
    security_info = SerializeSecurityInfo(
        cert_id, ssl_info.cert_status, ssl_info.security_bits,
        ssl_info.connection_status);
  }

  if (info->resource_handler()->OnResponseCompleted(info->GetRequestID(),
                                                    request->status(),
                                                    security_info)) {

    RemovePendingRequest(info->GetChildID(), info->GetRequestID());
  }
}
