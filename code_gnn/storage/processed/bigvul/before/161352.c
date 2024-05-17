Response NetworkHandler::SetUserAgentOverride(const std::string& user_agent) {
  if (user_agent.find('\n') != std::string::npos ||
      user_agent.find('\r') != std::string::npos ||
      user_agent.find('\0') != std::string::npos) {
    return Response::InvalidParams("Invalid characters found in userAgent");
  }
  user_agent_ = user_agent;
  return Response::FallThrough();
}
