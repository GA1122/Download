  std::unique_ptr<content::URLLoaderInterceptor> SetupRequestFailForURL(
      const GURL& url) {
    return URLLoaderInterceptor::SetupRequestFailForURL(url,
                                                        net::ERR_DNS_TIMED_OUT);
  }
