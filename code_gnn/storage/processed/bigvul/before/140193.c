void GaiaCookieManagerService::ExternalCcResultFetcher::OnURLFetchComplete(
    const net::URLFetcher* source) {
  const GURL& url = source->GetOriginalURL();
  const net::URLRequestStatus& status = source->GetStatus();
  int response_code = source->GetResponseCode();
  if (status.is_success() && response_code == net::HTTP_OK &&
      fetchers_.count(url) > 0) {
    std::string data;
    source->GetResponseAsString(&data);
    if (data.size() > 16)
      data.resize(16);
    results_[fetchers_[url].first] = data;

    DCHECK_EQ(source, fetchers_[url].second);
    fetchers_.erase(url);
    delete source;

    if (fetchers_.empty()) {
      CleanupTransientState();
      GetCheckConnectionInfoCompleted(true);
    }
  }
}
