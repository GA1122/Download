URLFetcher* WebSocketExperimentTask::Context::CreateURLFetcher(
    const Config& config, URLFetcher::Delegate* delegate) {
  URLRequestContextGetter* getter =
      Profile::GetDefaultRequestContext();
  if (!getter)
    return NULL;
  URLFetcher* fetcher =
      new URLFetcher(config.http_url, URLFetcher::GET, delegate);
  fetcher->set_request_context(getter);
  fetcher->set_load_flags(
      net::LOAD_BYPASS_CACHE | net::LOAD_DISABLE_CACHE |
      net::LOAD_DO_NOT_SEND_COOKIES | net::LOAD_DO_NOT_SEND_AUTH_DATA |
      net::LOAD_IGNORE_CERT_AUTHORITY_INVALID);
  return fetcher;
}
