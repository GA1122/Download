bool SyncManager::SyncInternal::OpenDirectory() {
  DCHECK(!initialized_) << "Should only happen once";

  change_observer_ =
      browser_sync::MakeWeakHandle(js_mutation_event_observer_.AsWeakPtr());
  WeakHandle<syncable::TransactionObserver> transaction_observer(
      browser_sync::MakeWeakHandle(js_mutation_event_observer_.AsWeakPtr()));

  syncable::DirOpenResult open_result = syncable::NOT_INITIALIZED;
  if (testing_mode_ == TEST_IN_MEMORY) {
    open_result = directory()->OpenInMemoryForTest(
        username_for_share(), this, transaction_observer);
  } else {
    open_result = directory()->Open(
        database_path_, username_for_share(), this, transaction_observer);
  }
  if (open_result != syncable::OPENED) {
    LOG(ERROR) << "Could not open share for:" << username_for_share();
    return false;
  }

  connection_manager()->set_client_id(directory()->cache_guid());
  return true;
}
