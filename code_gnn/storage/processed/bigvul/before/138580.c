void WriteFromUrlOperation::OnURLFetchDownloadProgress(
    const net::URLFetcher* source,
    int64_t current,
    int64_t total,
    int64_t current_network_bytes) {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);

  if (IsCancelled()) {
    url_fetcher_.reset(NULL);
  }

  int progress = (kProgressComplete * current) / total;

  SetProgress(progress);
}
