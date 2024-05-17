void AppCacheUpdateJob::LoadFromNewestCacheFailed(
    const GURL& url, AppCacheResponseInfo* response_info) {
  if (internal_state_ == CACHE_FAILURE)
    return;

  urls_to_fetch_.push_front(UrlToFetch(url, true, response_info));
  FetchUrls();
}
