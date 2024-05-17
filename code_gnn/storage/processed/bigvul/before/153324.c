 SkColor Tab::GetAlertIndicatorColor(TabAlertState state) const {
  const ui::ThemeProvider* theme_provider = GetThemeProvider();
  if (!theme_provider)
    return button_color_;

  switch (state) {
    case TabAlertState::AUDIO_PLAYING:
    case TabAlertState::AUDIO_MUTING:
      return theme_provider->GetColor(ThemeProperties::COLOR_TAB_ALERT_AUDIO);
    case TabAlertState::MEDIA_RECORDING:
    case TabAlertState::DESKTOP_CAPTURING:
      return theme_provider->GetColor(
          ThemeProperties::COLOR_TAB_ALERT_RECORDING);
    case TabAlertState::TAB_CAPTURING:
      return theme_provider->GetColor(
          ThemeProperties::COLOR_TAB_ALERT_CAPTURING);
    case TabAlertState::PIP_PLAYING:
      return theme_provider->GetColor(ThemeProperties::COLOR_TAB_PIP_PLAYING);
    case TabAlertState::BLUETOOTH_CONNECTED:
    case TabAlertState::USB_CONNECTED:
    case TabAlertState::SERIAL_CONNECTED:
    case TabAlertState::NONE:
    case TabAlertState::VR_PRESENTING_IN_HEADSET:
      return button_color_;
    default:
      NOTREACHED();
      return button_color_;
  }
}
