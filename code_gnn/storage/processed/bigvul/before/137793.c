base::string16 AssembleTabAccessibilityLabel(const base::string16& title,
                                             bool is_crashed,
                                             bool is_network_error,
                                             TabAlertState alert_state) {
  if (is_crashed)
    return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_CRASHED_FORMAT, title);

  if (is_network_error) {
    return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_NETWORK_ERROR_FORMAT,
                                      title);
  }

  switch (alert_state) {
    case TabAlertState::AUDIO_PLAYING:
      return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_AUDIO_PLAYING_FORMAT,
                                        title);
    case TabAlertState::USB_CONNECTED:
      return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_USB_CONNECTED_FORMAT,
                                        title);
    case TabAlertState::BLUETOOTH_CONNECTED:
      return l10n_util::GetStringFUTF16(
          IDS_TAB_AX_LABEL_BLUETOOTH_CONNECTED_FORMAT, title);
    case TabAlertState::MEDIA_RECORDING:
      return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_MEDIA_RECORDING_FORMAT,
                                        title);
    case TabAlertState::AUDIO_MUTING:
      return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_AUDIO_MUTING_FORMAT,
                                        title);
    case TabAlertState::TAB_CAPTURING:
      return l10n_util::GetStringFUTF16(IDS_TAB_AX_LABEL_TAB_CAPTURING_FORMAT,
                                        title);
    case TabAlertState::NONE:
      return title;
  }

  NOTREACHED();
  return base::string16();
}
