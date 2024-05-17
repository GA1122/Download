void DataReductionProxyConfig::SetProxyConfig(bool enabled, bool at_startup) {
  DCHECK(thread_checker_.CalledOnValidThread());
  enabled_by_user_ = enabled;
  network_properties_manager_->OnChangeInNetworkID(GetCurrentNetworkID());

  ReloadConfig();

  if (enabled_by_user_) {
    HandleCaptivePortal();

    SecureProxyCheck(
        base::Bind(&DataReductionProxyConfig::HandleSecureProxyCheckResponse,
                   base::Unretained(this)));
  }
  network_properties_manager_->ResetWarmupURLFetchMetrics();
  FetchWarmupProbeURL();
}
