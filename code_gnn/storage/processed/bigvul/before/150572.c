void DataReductionProxyConfig::UpdateConfigForTesting(
    bool enabled,
    bool secure_proxies_allowed,
    bool insecure_proxies_allowed) {
  enabled_by_user_ = enabled;
  network_properties_manager_->ResetWarmupURLFetchMetrics();
  network_properties_manager_->SetIsSecureProxyDisallowedByCarrier(
      !secure_proxies_allowed);
  if (!insecure_proxies_allowed !=
          network_properties_manager_->HasWarmupURLProbeFailed(
              false  , true  )) {
    network_properties_manager_->SetHasWarmupURLProbeFailed(
        false  , true  ,
        !insecure_proxies_allowed);
  }
}
