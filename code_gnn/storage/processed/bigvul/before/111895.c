bool ProfileSyncService::IsCryptographerReady(
    const sync_api::BaseTransaction* trans) const {
  return backend_.get() && backend_->IsCryptographerReady(trans);
}
