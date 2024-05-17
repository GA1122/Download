IndexedDBTransaction::GetPendingChangesForConnection(int32_t connection_id) {
  auto it = connection_changes_map_.find(connection_id);
  if (it != connection_changes_map_.end())
    return &it->second;
  return nullptr;
}
