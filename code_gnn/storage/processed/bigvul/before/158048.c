void LocalFrameClientImpl::DidCreateNewDocument() {
  if (web_frame_->Client())
    web_frame_->Client()->DidCreateNewDocument();
}
