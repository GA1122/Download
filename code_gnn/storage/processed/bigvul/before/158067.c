void LocalFrameClientImpl::DispatchDidChangeThemeColor() {
  if (web_frame_->Client())
    web_frame_->Client()->DidChangeThemeColor();
}
