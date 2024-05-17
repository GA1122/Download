String FrameLoader::UserAgent() const {
  String user_agent = Client()->UserAgent();
  probe::ApplyUserAgentOverride(probe::ToCoreProbeSink(frame_->GetDocument()),
                                &user_agent);
  return user_agent;
}
