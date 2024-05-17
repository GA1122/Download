void AppCacheUpdateJob::FetchUrls() {
  DCHECK(internal_state_ == DOWNLOADING);

  while (pending_url_fetches_.size() < kMaxConcurrentUrlFetches &&
         !urls_to_fetch_.empty()) {
    UrlToFetch url_to_fetch = urls_to_fetch_.front();
    urls_to_fetch_.pop_front();

    auto it = url_file_list_.find(url_to_fetch.url);
    DCHECK(it != url_file_list_.end());
    AppCacheEntry& entry = it->second;
    if (ShouldSkipUrlFetch(entry)) {
      NotifyAllProgress(url_to_fetch.url);
      ++url_fetches_completed_;
    } else if (AlreadyFetchedEntry(url_to_fetch.url, entry.types())) {
      NotifyAllProgress(url_to_fetch.url);
      ++url_fetches_completed_;   
    } else if (!url_to_fetch.storage_checked &&
               MaybeLoadFromNewestCache(url_to_fetch.url, entry)) {
    } else {
      URLFetcher* fetcher =
          new URLFetcher(url_to_fetch.url, URLFetcher::URL_FETCH, this,
                         kAppCacheFetchBufferSize);
      if (url_to_fetch.existing_response_info.get() &&
          group_->newest_complete_cache()) {
        AppCacheEntry* existing_entry =
            group_->newest_complete_cache()->GetEntry(url_to_fetch.url);
        DCHECK(existing_entry);
        DCHECK(existing_entry->response_id() ==
               url_to_fetch.existing_response_info->response_id());
        fetcher->set_existing_response_headers(
            url_to_fetch.existing_response_info->http_response_info()
                .headers.get());
        fetcher->set_existing_entry(*existing_entry);
      }
      fetcher->Start();
      pending_url_fetches_.insert(
          PendingUrlFetches::value_type(url_to_fetch.url, fetcher));
    }
  }
}
