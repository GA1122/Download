bool AppCacheUpdateJob::AlreadyFetchedEntry(const GURL& url,
                                            int entry_type) {
  DCHECK(internal_state_ == DOWNLOADING || internal_state_ == NO_UPDATE);
  AppCacheEntry* existing =
      inprogress_cache_.get() ? inprogress_cache_->GetEntry(url)
                              : group_->newest_complete_cache()->GetEntry(url);
  if (existing) {
    existing->add_types(entry_type);
    return true;
  }
  return false;
}
