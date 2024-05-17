void ClientSession::RestoreEventState() {
  std::set<int>::iterator i;
  for (i = pressed_keys_.begin(); i != pressed_keys_.end(); ++i) {
    KeyEvent key;
    key.set_keycode(*i);
    key.set_pressed(false);
    input_stub_->InjectKeyEvent(key);
  }
  pressed_keys_.clear();

  for (int i = 1; i < MouseEvent::BUTTON_MAX; i++) {
    if (remote_mouse_button_state_ & (1 << (i - 1))) {
      MouseEvent mouse;
      mouse.set_x(remote_mouse_pos_.x());
      mouse.set_y(remote_mouse_pos_.y());
      mouse.set_button((MouseEvent::MouseButton)i);
      mouse.set_button_down(false);
      input_stub_->InjectMouseEvent(mouse);
    }
  }
  remote_mouse_button_state_ = 0;
}
