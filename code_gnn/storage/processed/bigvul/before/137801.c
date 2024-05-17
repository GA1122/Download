gfx::Image GetTabAlertIndicatorImage(TabAlertState alert_state,
                                     SkColor button_color) {
  const gfx::VectorIcon* icon = nullptr;
  switch (alert_state) {
    case TabAlertState::AUDIO_PLAYING:
      icon = &kTabAudioIcon;
      break;
    case TabAlertState::AUDIO_MUTING:
      icon = &kTabAudioMutingIcon;
      break;
    case TabAlertState::MEDIA_RECORDING:
      icon = &kTabMediaRecordingIcon;
      break;
    case TabAlertState::TAB_CAPTURING:
      icon = &kTabMediaCapturingIcon;
      break;
    case TabAlertState::BLUETOOTH_CONNECTED:
      icon = &kTabBluetoothConnectedIcon;
      break;
    case TabAlertState::USB_CONNECTED:
      icon = &kTabUsbConnectedIcon;
      break;
    case TabAlertState::NONE:
      return gfx::Image();
  }
  DCHECK(icon);
  return gfx::Image(gfx::CreateVectorIcon(*icon, 16, button_color));
}
