net::ProxyList DataReductionProxyConfig::GetAllConfiguredProxies() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return config_values_->GetAllConfiguredProxies();
}
