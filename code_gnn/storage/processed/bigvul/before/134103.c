int EventFlagsFromXState(unsigned int state) {
  return (state & LockMask ? ui::EF_CAPS_LOCK_DOWN : 0) |
      (state & ControlMask ? ui::EF_CONTROL_DOWN : 0) |
      (state & ShiftMask ? ui::EF_SHIFT_DOWN : 0) |
      (state & Mod1Mask ? ui::EF_ALT_DOWN : 0) |
      (state & Button1Mask ? ui::EF_LEFT_MOUSE_BUTTON : 0) |
      (state & Button2Mask ? ui::EF_MIDDLE_MOUSE_BUTTON : 0) |
      (state & Button3Mask ? ui::EF_RIGHT_MOUSE_BUTTON : 0);
}
