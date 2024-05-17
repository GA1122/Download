String LocalFrameClientImpl::UserAgent() {
  WebString override =
      web_frame_->Client() ? web_frame_->Client()->UserAgentOverride() : "";
  if (!override.IsEmpty())
    return override;

  if (user_agent_.IsEmpty())
    user_agent_ = Platform::Current()->UserAgent();
  return user_agent_;
}
