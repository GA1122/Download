DataReductionProxyConfig::FindConfiguredDataReductionProxy(
    const net::ProxyServer& proxy_server) const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return config_values_->FindConfiguredDataReductionProxy(proxy_server);
}
