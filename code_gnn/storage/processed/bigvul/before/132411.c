bool IsKeyboardOverscrollEnabled() {
  if (!IsKeyboardEnabled())
    return false;

  if (g_accessibility_keyboard_enabled)
    return false;

  if (g_keyboard_overscroll_override != KEYBOARD_OVERSCROLL_OVERRIDE_NONE) {
    return g_keyboard_overscroll_override ==
        KEYBOARD_OVERSCROLL_OVERRIDE_ENABLED;
  }

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableVirtualKeyboardOverscroll)) {
    return false;
  }
  return true;
}
