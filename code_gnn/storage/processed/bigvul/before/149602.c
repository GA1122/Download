void PreconnectManager::PreconnectUrl(
    const GURL& url,
    int num_sockets,
    bool allow_credentials,
    const net::NetworkIsolationKey& network_isolation_key) const {
  DCHECK(url.GetOrigin() == url);
  DCHECK(url.SchemeIsHTTPOrHTTPS());
  if (observer_)
    observer_->OnPreconnectUrl(url, num_sockets, allow_credentials);

  auto* network_context = GetNetworkContext();
  if (!network_context)
    return;

  network_context->PreconnectSockets(num_sockets, url, allow_credentials,
                                     network_isolation_key);
}
