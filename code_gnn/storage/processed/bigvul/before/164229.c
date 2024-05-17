void AppCacheUpdateJob::AddAllAssociatedHostsToNotifier(
    HostNotifier* host_notifier) {
  if (inprogress_cache_.get()) {
    DCHECK(internal_state_ == DOWNLOADING || internal_state_ == CACHE_FAILURE);
    host_notifier->AddHosts(inprogress_cache_->associated_hosts());
  }

  for (AppCache* cache : group_->old_caches())
    host_notifier->AddHosts(cache->associated_hosts());

  AppCache* newest_cache = group_->newest_complete_cache();
  if (newest_cache)
    host_notifier->AddHosts(newest_cache->associated_hosts());
}
