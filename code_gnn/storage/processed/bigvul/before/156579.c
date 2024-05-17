void SessionStore::DeleteAllDataAndMetadata() {
  session_tracker_.Clear();
  return store_->DeleteAllDataAndMetadata(base::DoNothing());
}
