void DesktopWindowTreeHostX11::OnFocusEvent(bool focus_in,
                                            int mode,
                                            int detail) {
  if (detail == NotifyInferior)
    return;

  bool notify_grab = mode == NotifyGrab || mode == NotifyUngrab;

  BeforeActivationStateChanged();


  if (!notify_grab && detail != NotifyPointer)
    has_window_focus_ = focus_in;

  if (!notify_grab && has_pointer_) {
    switch (detail) {
      case NotifyAncestor:
      case NotifyVirtual:
        has_pointer_focus_ = !focus_in;
        break;
      case NotifyPointer:
        has_pointer_focus_ = focus_in;
        break;
      case NotifyNonlinear:
      case NotifyNonlinearVirtual:
        has_pointer_focus_ = false;
        break;
      default:
        break;
    }
  }

  ignore_keyboard_input_ = false;

  AfterActivationStateChanged();
}
