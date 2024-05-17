std::unique_ptr<ProxyLookupClientImpl> PreconnectManager::LookupProxyForUrl(
    const GURL& url,
    ProxyLookupCallback callback) const {
  DCHECK(url.GetOrigin() == url);
  DCHECK(url.SchemeIsHTTPOrHTTPS());

  auto* network_context = GetNetworkContext();
  if (!network_context) {
    std::move(callback).Run(false);
    return nullptr;
  }

  return std::make_unique<ProxyLookupClientImpl>(url, std::move(callback),
                                                 network_context);
}
