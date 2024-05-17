  void CompleteProxyLookup(const GURL& url,
                           const base::Optional<net::ProxyInfo>& result) {
    if (IsHangingHost(url))
      return;

    auto it = proxy_lookup_clients_.find(url);
    if (it == proxy_lookup_clients_.end()) {
      ADD_FAILURE() << url.spec() << " wasn't found";
      return;
    }
    it->second->OnProxyLookupComplete(net::ERR_FAILED, result);
    proxy_lookup_clients_.erase(it);
    base::RunLoop().RunUntilIdle();
  }
