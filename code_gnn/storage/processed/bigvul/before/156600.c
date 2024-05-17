bool SessionStore::StorageKeyMatchesLocalSession(
    const std::string& storage_key) const {
  std::string session_tag;
  int tab_node_id;
  bool success = DecodeStorageKey(storage_key, &session_tag, &tab_node_id);
  DCHECK(success);
  return session_tag == local_session_info_.session_tag;
}
