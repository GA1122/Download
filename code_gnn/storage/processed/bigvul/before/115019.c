void SendMouseClick(int flags) {
  ui_controls::MouseButton button = ui_controls::LEFT;
  if ((flags & ui::EF_LEFT_MOUSE_BUTTON) ==
      ui::EF_LEFT_MOUSE_BUTTON) {
    button = ui_controls::LEFT;
  } else if ((flags & ui::EF_RIGHT_MOUSE_BUTTON) ==
             ui::EF_RIGHT_MOUSE_BUTTON) {
    button = ui_controls::RIGHT;
  } else if ((flags & ui::EF_MIDDLE_MOUSE_BUTTON) ==
             ui::EF_MIDDLE_MOUSE_BUTTON) {
    button = ui_controls::MIDDLE;
  } else {
    NOTREACHED();
  }

  ui_controls::SendMouseClick(button);
}
