void IndexedDBDatabase::CallUpgradeTransactionStartedForTesting(
    int64_t old_version) {
  DCHECK(active_request_);
  active_request_->UpgradeTransactionStarted(old_version);
}
