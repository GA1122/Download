std::string SessionStore::WriteBatch::DeleteLocalTabWithoutUpdatingTracker(
    int tab_node_id) {
  std::string storage_key =
      EncodeStorageKey(session_tracker_->GetLocalSessionTag(), tab_node_id);
  batch_->DeleteData(storage_key);
  return storage_key;
}
