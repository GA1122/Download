void DisplaySourceCustomBindings::OnSessionStarted(
    int sink_id,
    int call_id,
    bool success,
    const std::string& error_message) {
  CHECK(GetDisplaySession(sink_id));
  if (!success) {
    session_map_.erase(sink_id);
  }
  OnCallCompleted(call_id, success, error_message);
}
