void DataReductionProxyConfig::ContinueNetworkChanged(
    const std::string& network_id) {
  network_properties_manager_->OnChangeInNetworkID(network_id);

  ReloadConfig();

  FetchWarmupProbeURL();

  if (enabled_by_user_) {
    HandleCaptivePortal();
    SecureProxyCheck(
        base::Bind(&DataReductionProxyConfig::HandleSecureProxyCheckResponse,
                   base::Unretained(this)));
  }
}
