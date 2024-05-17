bool DataReductionProxyConfig::IsProxyBypassed(
    const net::ProxyRetryInfoMap& retry_map,
    const net::ProxyServer& proxy_server,
    base::TimeDelta* retry_delay) const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return IsProxyBypassedAtTime(retry_map, proxy_server, GetTicksNow(),
                               retry_delay);
}
