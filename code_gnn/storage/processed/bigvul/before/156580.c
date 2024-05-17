void SessionStore::WriteBatch::DeleteForeignEntityAndUpdateTracker(
    const std::string& storage_key) {
  std::string session_tag;
  int tab_node_id;
  bool success = DecodeStorageKey(storage_key, &session_tag, &tab_node_id);
  DCHECK(success);
  DCHECK_NE(session_tag, session_tracker_->GetLocalSessionTag());

  if (tab_node_id == TabNodePool::kInvalidTabNodeID) {
    session_tracker_->DeleteForeignSession(session_tag);
  } else {
    session_tracker_->DeleteForeignTab(session_tag, tab_node_id);
  }

  batch_->DeleteData(storage_key);
}
