URLFetcher* TestURLFetcherFactory::CreateURLFetcher(
    int id,
    const GURL& url,
    URLFetcher::RequestType request_type,
    URLFetcher::Delegate* d) {
  TestURLFetcher* fetcher = new TestURLFetcher(id, url, request_type, d);
  fetchers_[id] = fetcher;
  return fetcher;
}
