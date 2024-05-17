bool DataReductionProxyConfig::ContainsDataReductionProxy(
    const net::ProxyConfig::ProxyRules& proxy_rules) const {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (proxy_rules.type != net::ProxyConfig::ProxyRules::Type::PROXY_LIST_PER_SCHEME)
    return false;

  const net::ProxyList* http_proxy_list =
      proxy_rules.MapUrlSchemeToProxyList("http");
  if (http_proxy_list && !http_proxy_list->IsEmpty() &&
      FindConfiguredDataReductionProxy(http_proxy_list->Get())) {
    return true;
  }

  return false;
}
