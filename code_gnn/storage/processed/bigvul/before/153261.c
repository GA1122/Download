void DesktopWindowTreeHostX11::OnCrossingEvent(bool enter,
                                               bool focus_in_window_or_ancestor,
                                               int mode,
                                               int detail) {
  if (detail == NotifyInferior)
    return;

  BeforeActivationStateChanged();

  if (mode == NotifyGrab)
    has_pointer_grab_ = enter;
  else if (mode == NotifyUngrab)
    has_pointer_grab_ = false;

  has_pointer_ = enter;
  if (focus_in_window_or_ancestor && !has_window_focus_) {
    has_pointer_focus_ = has_pointer_;
  }

  AfterActivationStateChanged();
}
