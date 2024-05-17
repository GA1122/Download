void AppCacheUpdateJob::CancelAllMasterEntryFetches(
    const AppCacheErrorDetails& error_details) {

  for (PendingUrlFetches::iterator it = master_entry_fetches_.begin();
       it != master_entry_fetches_.end(); ++it) {
    delete it->second;
    master_entries_to_fetch_.insert(it->first);   
  }
  master_entry_fetches_.clear();

  master_entries_completed_ += master_entries_to_fetch_.size();

  HostNotifier host_notifier;
  while (!master_entries_to_fetch_.empty()) {
    const GURL& url = *master_entries_to_fetch_.begin();
    PendingMasters::iterator found = pending_master_entries_.find(url);
    DCHECK(found != pending_master_entries_.end());
    PendingHosts& hosts = found->second;
    for (PendingHosts::iterator host_it = hosts.begin();
         host_it != hosts.end(); ++host_it) {
      AppCacheHost* host = *host_it;
      host->AssociateNoCache(GURL());
      host_notifier.AddHost(host);
      host->RemoveObserver(this);
    }
    hosts.clear();

    master_entries_to_fetch_.erase(master_entries_to_fetch_.begin());
  }
  host_notifier.SendErrorNotifications(error_details);
}
