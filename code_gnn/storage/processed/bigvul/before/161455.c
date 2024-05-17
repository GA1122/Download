void TargetHandler::AutoAttach(DevToolsAgentHost* host,
                               bool waiting_for_debugger) {
  std::string session_id = Session::Attach(this, host, waiting_for_debugger);
  auto_attached_sessions_[host] = attached_sessions_[session_id].get();
}
