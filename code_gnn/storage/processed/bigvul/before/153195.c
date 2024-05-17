void DesktopWindowTreeHostX11::AfterActivationStateChanged() {
  if (had_pointer_grab_ && !has_pointer_grab_)
    dispatcher()->OnHostLostMouseGrab();

  bool had_pointer_capture = had_pointer_ || had_pointer_grab_;
  bool has_pointer_capture = has_pointer_ || has_pointer_grab_;
  if (had_pointer_capture && !has_pointer_capture)
    OnHostLostWindowCapture();

  if (!was_active_ && IsActive()) {
    FlashFrame(false);
    open_windows().remove(xwindow_);
    open_windows().insert(open_windows().begin(), xwindow_);
  }

  if (was_active_ != IsActive()) {
    desktop_native_widget_aura_->HandleActivationChanged(IsActive());
    native_widget_delegate_->AsWidget()->GetRootView()->SchedulePaint();
  }
}
