String FrameLoader::UserAgent() const {
  String user_agent = Client()->UserAgent();
  probe::applyUserAgentOverride(frame_->GetDocument(), &user_agent);
  return user_agent;
}
