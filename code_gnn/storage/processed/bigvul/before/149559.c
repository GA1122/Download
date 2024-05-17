  bool HasProxyBeenLookedUp(const GURL& url) {
    return base::Contains(successful_proxy_lookups_, url.GetOrigin()) ||
           base::Contains(unsuccessful_proxy_lookups_, url.GetOrigin());
  }
