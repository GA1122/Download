void LocalFrameClientImpl::LoadErrorPage(int reason) {
  if (web_frame_->Client())
    web_frame_->Client()->LoadErrorPage(reason);
}
