void DataReductionProxyConfig::ReloadConfig() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(configurator_);

  if (enabled_by_user_ && !params::IsIncludedInHoldbackFieldTrial() &&
      !config_values_->proxies_for_http().empty()) {
    configurator_->Enable(*network_properties_manager_,
                          config_values_->proxies_for_http());
  } else {
    configurator_->Disable();
  }
}
