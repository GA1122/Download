std::string SessionStore::GetClientTag(const SessionSpecifics& specifics) {
  DCHECK(AreValidSpecifics(specifics));

  if (specifics.has_header()) {
    return specifics.session_tag();
  }

  DCHECK(specifics.has_tab());
  return TabNodeIdToClientTag(specifics.session_tag(), specifics.tab_node_id());
}
