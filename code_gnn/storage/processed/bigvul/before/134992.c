void AppCacheUpdateJob::ClearPendingMasterEntries() {
  for (PendingMasters::iterator it = pending_master_entries_.begin();
       it != pending_master_entries_.end(); ++it) {
    PendingHosts& hosts = it->second;
    for (PendingHosts::iterator host_it = hosts.begin();
         host_it != hosts.end(); ++host_it) {
      (*host_it)->RemoveObserver(this);
    }
  }

  pending_master_entries_.clear();
}
