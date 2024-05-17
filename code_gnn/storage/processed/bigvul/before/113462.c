void InputMethodIBus::PendingKeyEventImpl::ProcessPostIME(bool handled) {
  if (!input_method_)
    return;

  if (x_event_.type == KeyPress || x_event_.type == KeyRelease) {
    input_method_->ProcessKeyEventPostIME(reinterpret_cast<XEvent*>(&x_event_),
                                          ibus_keyval_,
                                          handled);
    return;
  }

}
