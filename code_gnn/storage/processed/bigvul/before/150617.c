void DataReductionProxyIOData::UpdateCustomProxyConfig() {
  if (!proxy_config_client_)
    return;

  proxy_config_client_->OnCustomProxyConfigUpdated(CreateCustomProxyConfig(
      !base::FeatureList::IsEnabled(
          features::kDataReductionProxyDisableProxyFailedWarmup),
      config_->GetProxiesForHttp()));
}
