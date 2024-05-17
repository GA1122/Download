  void PressMouseButton(ui::EventFlags mouse_button_flags, int extra_flags) {
    ui::MouseEvent press(ui::ET_MOUSE_PRESSED, mouse_position_, mouse_position_,
                         ui::EventTimeForNow(), mouse_button_flags,
                         mouse_button_flags | extra_flags);
    textfield_->OnMousePressed(press);
  }
