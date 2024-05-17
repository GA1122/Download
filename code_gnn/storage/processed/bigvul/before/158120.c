void LocalFrameClientImpl::SetMouseCapture(bool capture) {
  web_frame_->Client()->SetMouseCapture(capture);
}
