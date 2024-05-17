RenderFrameImpl::CreateWorkerContentSettingsClient() {
  if (!frame_ || !frame_->View())
    return nullptr;
  return GetContentClient()->renderer()->CreateWorkerContentSettingsClient(
      this);
}
