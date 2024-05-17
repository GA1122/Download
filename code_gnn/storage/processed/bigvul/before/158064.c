void LocalFrameClientImpl::DidUpdateCurrentHistoryItem() {
  if (web_frame_->Client())
    web_frame_->Client()->DidUpdateCurrentHistoryItem();
}
