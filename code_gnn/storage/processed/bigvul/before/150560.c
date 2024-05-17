void DataReductionProxyConfig::HandleSecureProxyCheckResponse(
    const std::string& response,
    int net_status,
    int http_response_code) {
  bool success_response =
      base::StartsWith(response, "OK", base::CompareCase::SENSITIVE);

  if (net_status != net::OK) {
    if (net_status == net::ERR_INTERNET_DISCONNECTED) {
      RecordSecureProxyCheckFetchResult(INTERNET_DISCONNECTED);
      return;
    }
    base::UmaHistogramSparse(kUMAProxyProbeURLNetError, std::abs(net_status));
  }

  bool secure_proxy_allowed_past =
      !network_properties_manager_->IsSecureProxyDisallowedByCarrier();
  network_properties_manager_->SetIsSecureProxyDisallowedByCarrier(
      !success_response);
  if (!enabled_by_user_)
    return;

  if (!network_properties_manager_->IsSecureProxyDisallowedByCarrier() !=
      secure_proxy_allowed_past)
    ReloadConfig();

  if (secure_proxy_allowed_past &&
      !network_properties_manager_->IsSecureProxyDisallowedByCarrier()) {
    RecordSecureProxyCheckFetchResult(SUCCEEDED_PROXY_ALREADY_ENABLED);
  } else if (secure_proxy_allowed_past &&
             network_properties_manager_->IsSecureProxyDisallowedByCarrier()) {
    RecordSecureProxyCheckFetchResult(FAILED_PROXY_DISABLED);
  } else if (!secure_proxy_allowed_past &&
             !network_properties_manager_->IsSecureProxyDisallowedByCarrier()) {
    RecordSecureProxyCheckFetchResult(SUCCEEDED_PROXY_ENABLED);
  } else {
    DCHECK(!secure_proxy_allowed_past &&
           network_properties_manager_->IsSecureProxyDisallowedByCarrier());
    RecordSecureProxyCheckFetchResult(FAILED_PROXY_ALREADY_DISABLED);
  }
}
