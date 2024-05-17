void HWNDMessageHandler::SetVisibilityChangedAnimationsEnabled(bool enabled) {
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    int dwm_value = enabled ? FALSE : TRUE;
    DwmSetWindowAttribute(
        hwnd(), DWMWA_TRANSITIONS_FORCEDISABLED, &dwm_value, sizeof(dwm_value));
  }
}
