void LocalFrameClientImpl::DispatchWillCommitProvisionalLoad() {
  if (web_frame_->Client())
    web_frame_->Client()->WillCommitProvisionalLoad();
}
