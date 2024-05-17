void DevToolsDataSource::StartCustomDataRequest(
    const GURL& url,
    const content::URLDataSource::GotDataCallback& callback) {
  if (!url.is_valid()) {
    callback.Run(
        new base::RefCountedStaticMemory(kHttpNotFound, strlen(kHttpNotFound)));
    return;
  }
  net::URLFetcher* fetcher =
      net::URLFetcher::Create(url, net::URLFetcher::GET, this).release();
  pending_[fetcher] = callback;
  fetcher->SetRequestContext(request_context_.get());
  fetcher->SetLoadFlags(net::LOAD_DISABLE_CACHE);
  fetcher->Start();
}
