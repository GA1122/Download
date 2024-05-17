void DesktopWindowTreeHostX11::BeforeActivationStateChanged() {
  was_active_ = IsActive();
  had_pointer_ = has_pointer_;
  had_pointer_grab_ = has_pointer_grab_;
  had_window_focus_ = has_window_focus_;
}
