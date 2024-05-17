LocalFrameClientImpl::CreateWorkerContentSettingsClient() {
  DCHECK(web_frame_->Client());
  return web_frame_->Client()->CreateWorkerContentSettingsClient();
}
