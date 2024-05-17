std::string SessionStore::GetStorageKey(const SessionSpecifics& specifics) {
  DCHECK(AreValidSpecifics(specifics));
  return EncodeStorageKey(specifics.session_tag(), specifics.tab_node_id());
}
