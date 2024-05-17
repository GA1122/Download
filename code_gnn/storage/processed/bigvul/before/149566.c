  bool ProxyFound(const GURL& url) {
    return base::Contains(successful_proxy_lookups_, url.GetOrigin());
  }
