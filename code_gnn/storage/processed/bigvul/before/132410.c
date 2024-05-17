bool IsKeyboardEnabled() {
  if (g_accessibility_keyboard_enabled)
    return true;
  if (g_keyboard_show_override == keyboard::KEYBOARD_SHOW_OVERRIDE_DISABLED)
    return false;
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
             switches::kEnableVirtualKeyboard) ||
         g_touch_keyboard_enabled ||
         (g_keyboard_show_override == keyboard::KEYBOARD_SHOW_OVERRIDE_ENABLED);
}
