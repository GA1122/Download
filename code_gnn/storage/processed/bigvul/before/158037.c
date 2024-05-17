void LocalFrameClientImpl::DidAccessInitialDocument() {
  if (web_frame_->Client())
    web_frame_->Client()->DidAccessInitialDocument();
}
