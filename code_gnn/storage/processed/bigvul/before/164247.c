void AppCacheUpdateJob::FetchMasterEntries() {
  DCHECK(internal_state_ == NO_UPDATE || internal_state_ == DOWNLOADING);

  while (master_entry_fetches_.size() < kMaxConcurrentUrlFetches &&
         !master_entries_to_fetch_.empty()) {
    const GURL& url = *master_entries_to_fetch_.begin();

    if (AlreadyFetchedEntry(url, AppCacheEntry::MASTER)) {
      ++master_entries_completed_;   

      if (internal_state_ == NO_UPDATE) {
        DCHECK(!inprogress_cache_.get());
        AppCache* cache = group_->newest_complete_cache();
        auto found = pending_master_entries_.find(url);
        DCHECK(found != pending_master_entries_.end());
        PendingHosts& hosts = found->second;
        for (AppCacheHost* host : hosts)
          host->AssociateCompleteCache(cache);
      }
    } else {
      URLFetcher* fetcher = new URLFetcher(url, URLFetcher::MASTER_ENTRY_FETCH,
                                           this, kAppCacheFetchBufferSize);
      fetcher->Start();
      master_entry_fetches_.insert(PendingUrlFetches::value_type(url, fetcher));
    }

    master_entries_to_fetch_.erase(master_entries_to_fetch_.begin());
  }
}