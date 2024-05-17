void DevToolsSession::DispatchProtocolMessageToAgent(
    int call_id,
    const std::string& method,
    const std::string& message) {
  DCHECK(!browser_only_);
  if (ShouldSendOnIO(method)) {
    if (io_session_ptr_)
      io_session_ptr_->DispatchProtocolCommand(call_id, method, message);
  } else {
    if (session_ptr_)
      session_ptr_->DispatchProtocolCommand(call_id, method, message);
  }
}
