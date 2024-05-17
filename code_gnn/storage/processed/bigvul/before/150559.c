void DataReductionProxyConfig::HandleCaptivePortal() {
  DCHECK(thread_checker_.CalledOnValidThread());

  bool is_captive_portal = GetIsCaptivePortal();
  if (is_captive_portal == network_properties_manager_->IsCaptivePortal())
    return;
  network_properties_manager_->SetIsCaptivePortal(is_captive_portal);
  ReloadConfig();
}
