void AppCacheUpdateJob::DiscardInprogressCache() {
  if (stored_state_ == STORING) {
    inprogress_cache_ = nullptr;
    added_master_entries_.clear();
    return;
  }

  storage_->DoomResponses(manifest_url_, stored_response_ids_);

  if (!inprogress_cache_.get()) {
    if (group_ && group_->newest_complete_cache()) {
      for (auto& url : added_master_entries_)
        group_->newest_complete_cache()->RemoveEntry(url);
    }
    added_master_entries_.clear();
    return;
  }

  AppCache::AppCacheHosts& hosts = inprogress_cache_->associated_hosts();
  while (!hosts.empty())
    (*hosts.begin())->AssociateNoCache(GURL());

  inprogress_cache_ = nullptr;
  added_master_entries_.clear();
}
