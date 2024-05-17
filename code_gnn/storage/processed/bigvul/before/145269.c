DisplaySourceSession* DisplaySourceCustomBindings::GetDisplaySession(
    int sink_id) const {
  auto iter = session_map_.find(sink_id);
  if (iter != session_map_.end())
    return iter->second.get();
  return nullptr;
}
