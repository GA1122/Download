Response InspectorNetworkAgent::setUserAgentOverride(const String& user_agent) {
  if (user_agent.Contains('\n') || user_agent.Contains('\r') ||
      user_agent.Contains('\0')) {
    return Response::Error("Invalid characters found in userAgent");
  }
  state_->setString(NetworkAgentState::kUserAgentOverride, user_agent);
  return Response::OK();
}
