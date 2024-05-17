bool SessionStore::AreValidSpecifics(const SessionSpecifics& specifics) {
  if (specifics.session_tag().empty()) {
    return false;
  }
  if (specifics.has_tab()) {
    return specifics.tab_node_id() >= 0 && specifics.tab().tab_id() > 0;
  }
  if (specifics.has_header()) {
    std::set<int> session_tab_ids;
    for (const sync_pb::SessionWindow& window : specifics.header().window()) {
      for (int tab_id : window.tab()) {
        bool success = session_tab_ids.insert(tab_id).second;
        if (!success) {
          return false;
        }
      }
    }
    return !specifics.has_tab() &&
           specifics.tab_node_id() == TabNodePool::kInvalidTabNodeID;
  }
  return false;
}
