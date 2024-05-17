void DevToolsSession::DispatchProtocolResponse(
    const std::string& message,
    int call_id,
    const base::Optional<std::string>& state) {
  if (state.has_value())
    state_cookie_ = state.value();
  waiting_for_response_messages_.erase(call_id);
  client_->DispatchProtocolMessage(agent_host_, message);
}
