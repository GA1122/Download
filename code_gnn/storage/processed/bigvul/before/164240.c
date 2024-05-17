void AppCacheUpdateJob::ClearPendingMasterEntries() {
  for (auto& pair : pending_master_entries_) {
    PendingHosts& hosts = pair.second;
    for (AppCacheHost* host : hosts)
      host->RemoveObserver(this);
  }

  pending_master_entries_.clear();
}
