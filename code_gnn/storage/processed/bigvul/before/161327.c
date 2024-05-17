Response NetworkHandler::Disable() {
  enabled_ = false;
  user_agent_ = std::string();
  interception_handle_.reset();
  SetNetworkConditions(nullptr);
  extra_headers_.clear();
  return Response::FallThrough();
}
