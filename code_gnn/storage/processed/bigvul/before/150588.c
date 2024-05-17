bool DataReductionProxyConfigServiceClient::ShouldRetryDueToAuthFailure(
    const net::HttpRequestHeaders& request_headers,
    const net::HttpResponseHeaders* response_headers,
    const net::ProxyServer& proxy_server,
    const net::LoadTimingInfo& load_timing_info) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(response_headers);

  if (!config_->FindConfiguredDataReductionProxy(proxy_server))
    return false;

  if (response_headers->response_code() !=
      net::HTTP_PROXY_AUTHENTICATION_REQUIRED) {
    previous_request_failed_authentication_ = false;
    return false;
  }

  base::Optional<std::string> session_key =
      request_options_->GetSessionKeyFromRequestHeaders(request_headers);
  if ((session_key.has_value() ? session_key.value() : std::string()) !=
      request_options_->GetSecureSession()) {
    RecordAuthExpiredSessionKey(false);
    return true;
  }
  RecordAuthExpiredSessionKey(true);

  if (previous_request_failed_authentication_)
    GetBackoffEntry()->InformOfRequest(false);

  RecordAuthExpiredHistogram(true);
  previous_request_failed_authentication_ = true;
  InvalidateConfig();
  DCHECK(config_->GetProxiesForHttp().empty());

  if (fetch_in_progress_) {
    return true;
  }

  RetrieveConfig();

  if (!load_timing_info.send_start.is_null() &&
      !load_timing_info.request_start.is_null() &&
      !network_connection_tracker_->IsOffline() &&
      last_ip_address_change_ < load_timing_info.request_start) {
    UMA_HISTOGRAM_TIMES(
        "DataReductionProxy.ConfigService.AuthFailure.LatencyPenalty",
        base::TimeTicks::Now() - load_timing_info.request_start);
  }

  return true;
}
