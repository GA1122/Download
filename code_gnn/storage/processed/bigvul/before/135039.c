AppCacheUpdateJob::UrlToFetch::UrlToFetch(const GURL& url,
                                          bool checked,
                                          AppCacheResponseInfo* info)
    : url(url),
      storage_checked(checked),
      existing_response_info(info) {
}
