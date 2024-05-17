void DataReductionProxySettings::SetConfiguredProxies(
    const net::ProxyList& proxies) {
  DCHECK(thread_checker_.CalledOnValidThread());
  configured_proxies_ = proxies;
}
