  void ReleaseMouseButton(ui::EventFlags mouse_button_flags) {
    ui::MouseEvent release(ui::ET_MOUSE_RELEASED, mouse_position_,
                           mouse_position_, ui::EventTimeForNow(),
                           mouse_button_flags, mouse_button_flags);
    textfield_->OnMouseReleased(release);
  }
