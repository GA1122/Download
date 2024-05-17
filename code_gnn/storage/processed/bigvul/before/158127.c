void LocalFrameClientImpl::SuddenTerminationDisablerChanged(
    bool present,
    WebSuddenTerminationDisablerType type) {
  if (web_frame_->Client()) {
    web_frame_->Client()->SuddenTerminationDisablerChanged(present, type);
  }
}
