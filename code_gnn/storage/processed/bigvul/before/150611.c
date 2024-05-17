void DataReductionProxyIOData::SetProxyPrefs(bool enabled, bool at_startup) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  enabled_ = enabled;
  config_->SetProxyConfig(enabled, at_startup);
  if (config_client_) {
    config_client_->SetEnabled(enabled);
    if (enabled)
      config_client_->RetrieveConfig();
  }

  if (!enabled) {
    if (proxy_config_client_)
      proxy_config_client_->ClearBadProxiesCache();
  }
}
