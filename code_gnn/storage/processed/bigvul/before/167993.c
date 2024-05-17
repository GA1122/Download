FrameNavigationDisabler::~FrameNavigationDisabler() {
  frame_->EnableNavigation();
}
