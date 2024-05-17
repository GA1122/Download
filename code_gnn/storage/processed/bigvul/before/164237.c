void AppCacheUpdateJob::CancelAllMasterEntryFetches(
    const blink::mojom::AppCacheErrorDetails& error_details) {

  for (auto& pair : master_entry_fetches_) {
    delete pair.second;
    master_entries_to_fetch_.insert(pair.first);   
  }
  master_entry_fetches_.clear();

  master_entries_completed_ += master_entries_to_fetch_.size();

  HostNotifier host_notifier;
  while (!master_entries_to_fetch_.empty()) {
    const GURL& url = *master_entries_to_fetch_.begin();
    auto found = pending_master_entries_.find(url);
    DCHECK(found != pending_master_entries_.end());
    PendingHosts& hosts = found->second;
    for (AppCacheHost* host : hosts) {
      host->AssociateNoCache(GURL());
      host_notifier.AddHost(host);
      host->RemoveObserver(this);
    }
    hosts.clear();

    master_entries_to_fetch_.erase(master_entries_to_fetch_.begin());
  }
  host_notifier.SendErrorNotifications(error_details);
}
