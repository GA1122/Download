  std::unique_ptr<content::URLLoaderInterceptor> SetupRequestFailForURL(
      const GURL& url) {
    return std::make_unique<content::URLLoaderInterceptor>(base::BindRepeating(
        [](const GURL& url,
           content::URLLoaderInterceptor::RequestParams* params) {
          if (params->url_request.url != url)
            return false;
          network::URLLoaderCompletionStatus status;
          status.error_code = net::ERR_DNS_TIMED_OUT;
          params->client->OnComplete(status);
          return true;
        },
        url));
  }
