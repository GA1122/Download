int EventFlagsFromXFlags(unsigned int flags) {
  return (flags & LockMask ? ui::EF_CAPS_LOCK_DOWN : 0) |
      (flags & ControlMask ? ui::EF_CONTROL_DOWN : 0) |
      (flags & ShiftMask ? ui::EF_SHIFT_DOWN : 0) |
      (flags & Mod1Mask ? ui::EF_ALT_DOWN : 0) |
      (flags & Button1Mask ? ui::EF_LEFT_MOUSE_BUTTON : 0) |
      (flags & Button2Mask ? ui::EF_MIDDLE_MOUSE_BUTTON : 0) |
      (flags & Button3Mask ? ui::EF_RIGHT_MOUSE_BUTTON : 0);
}
