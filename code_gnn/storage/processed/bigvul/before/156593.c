std::string SessionStore::GetTabStorageKey(const std::string& session_tag,
                                           int tab_node_id) {
  DCHECK_GE(tab_node_id, 0);
  return EncodeStorageKey(session_tag, tab_node_id);
}
