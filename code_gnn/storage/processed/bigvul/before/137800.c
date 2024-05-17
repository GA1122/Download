gfx::Image GetTabAlertIndicatorAffordanceImage(TabAlertState alert_state,
                                               SkColor button_color) {
  switch (alert_state) {
    case TabAlertState::AUDIO_PLAYING:
      return GetTabAlertIndicatorImage(TabAlertState::AUDIO_MUTING,
                                       button_color);
    case TabAlertState::AUDIO_MUTING:
      return GetTabAlertIndicatorImage(TabAlertState::AUDIO_PLAYING,
                                       button_color);
    case TabAlertState::NONE:
    case TabAlertState::MEDIA_RECORDING:
    case TabAlertState::TAB_CAPTURING:
    case TabAlertState::BLUETOOTH_CONNECTED:
    case TabAlertState::USB_CONNECTED:
      return GetTabAlertIndicatorImage(alert_state, button_color);
  }
  NOTREACHED();
  return GetTabAlertIndicatorImage(alert_state, button_color);
}
