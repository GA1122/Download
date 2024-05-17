unsigned int XKeyEventKeyCode(ui::KeyboardCode key_code,
                              int flags,
                              Display* display) {
  const int keysym = XKeysymForWindowsKeyCode(key_code,
                                              flags & ui::EF_SHIFT_DOWN);
  return (keysym == XK_less) ? 59 : XKeysymToKeycode(display, keysym);
}
