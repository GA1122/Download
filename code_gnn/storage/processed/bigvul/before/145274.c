void DisplaySourceCustomBindings::OnSessionTerminated(int sink_id) {
  CHECK(GetDisplaySession(sink_id));
  session_map_.erase(sink_id);
  DispatchSessionTerminated(sink_id);
}
