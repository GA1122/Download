void DevToolsDataSource::StartRemoteDataRequest(
    const std::string& path,
    const content::URLDataSource::GotDataCallback& callback) {
  GURL url = GURL(kRemoteFrontendBase + path);
  CHECK_EQ(url.host(), kRemoteFrontendDomain);
  if (!url.is_valid()) {
    callback.Run(
        new base::RefCountedStaticMemory(kHttpNotFound, strlen(kHttpNotFound)));
    return;
  }
  net::URLFetcher* fetcher =
      net::URLFetcher::Create(url, net::URLFetcher::GET, this).release();
  pending_[fetcher] = callback;
  fetcher->SetRequestContext(request_context_.get());
  fetcher->Start();
}
