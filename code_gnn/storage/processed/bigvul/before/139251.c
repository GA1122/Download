void VRDisplay::ForceExitPresent() {
  if (display_) {
    display_->ExitPresent();
  }
  StopPresenting();
}
