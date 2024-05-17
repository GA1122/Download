DataReductionProxyIOData::CreateThrottleConfig() const {
  return DataReductionProxyThrottleManager::CreateConfig(
      config_->GetProxiesForHttp());
}
