WebEffectiveConnectionType LocalFrameClientImpl::GetEffectiveConnectionType() {
  if (web_frame_->Client())
    return web_frame_->Client()->GetEffectiveConnectionType();
  return WebEffectiveConnectionType::kTypeUnknown;
}
