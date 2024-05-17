bool SyncManager::SyncInternal::OpenDirectory() {
  DCHECK(!initialized_) << "Should only happen once";

  bool share_opened = dir_manager()->Open(username_for_share(), this);
  DCHECK(share_opened);
  if (!share_opened) {
    LOG(ERROR) << "Could not open share for:" << username_for_share();
    return false;
  }

  syncable::ScopedDirLookup lookup(dir_manager(), username_for_share());
  if (!lookup.good()) {
    NOTREACHED();
    return false;
  }

  connection_manager()->set_client_id(lookup->cache_guid());
  lookup->AddTransactionObserver(&js_transaction_observer_);
  return true;
}
