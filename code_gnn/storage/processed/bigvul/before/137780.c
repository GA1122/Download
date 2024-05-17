HeadlessWebContents::Builder::SetEnableBeginFrameControl(
    bool enable_begin_frame_control) {
  enable_begin_frame_control_ = enable_begin_frame_control;
  return *this;
}
