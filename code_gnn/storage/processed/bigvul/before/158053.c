void LocalFrameClientImpl::DidDisplayInsecureContent() {
  if (web_frame_->Client())
    web_frame_->Client()->DidDisplayInsecureContent();
}
