void EventConverterEvdevImpl::OnButtonChange(int code,
                                             bool down,
                                             const base::TimeDelta& timestamp) {
  if (code == BTN_SIDE)
    code = BTN_BACK;
  else if (code == BTN_EXTRA)
    code = BTN_FORWARD;

  int button_offset = code - BTN_MOUSE;
  if (mouse_button_state_.test(button_offset) == down)
    return;

  mouse_button_state_.set(button_offset, down);

  dispatcher_->DispatchMouseButtonEvent(MouseButtonEventParams(
      input_device_.id, cursor_->GetLocation(), code, down,
        true, timestamp));
}
