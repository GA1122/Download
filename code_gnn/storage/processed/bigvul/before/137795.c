bool CanToggleAudioMute(content::WebContents* contents) {
  switch (GetTabAlertStateForContents(contents)) {
    case TabAlertState::NONE:
    case TabAlertState::AUDIO_PLAYING:
    case TabAlertState::AUDIO_MUTING:
      return true;
    case TabAlertState::MEDIA_RECORDING:
    case TabAlertState::TAB_CAPTURING:
    case TabAlertState::BLUETOOTH_CONNECTED:
    case TabAlertState::USB_CONNECTED:
      return false;
  }
  NOTREACHED();
  return false;
}
