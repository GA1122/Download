base::string16 AssembleTabTooltipText(const base::string16& title,
                                      TabAlertState alert_state) {
  if (alert_state == TabAlertState::NONE)
    return title;

  base::string16 result = title;
  if (!result.empty())
    result.append(1, '\n');
  switch (alert_state) {
    case TabAlertState::AUDIO_PLAYING:
      result.append(
          l10n_util::GetStringUTF16(IDS_TOOLTIP_TAB_ALERT_STATE_AUDIO_PLAYING));
      break;
    case TabAlertState::AUDIO_MUTING:
      result.append(
          l10n_util::GetStringUTF16(IDS_TOOLTIP_TAB_ALERT_STATE_AUDIO_MUTING));
      break;
    case TabAlertState::MEDIA_RECORDING:
      result.append(l10n_util::GetStringUTF16(
          IDS_TOOLTIP_TAB_ALERT_STATE_MEDIA_RECORDING));
      break;
    case TabAlertState::TAB_CAPTURING:
      result.append(
          l10n_util::GetStringUTF16(IDS_TOOLTIP_TAB_ALERT_STATE_TAB_CAPTURING));
      break;
    case TabAlertState::BLUETOOTH_CONNECTED:
      result.append(l10n_util::GetStringUTF16(
          IDS_TOOLTIP_TAB_ALERT_STATE_BLUETOOTH_CONNECTED));
      break;
    case TabAlertState::USB_CONNECTED:
      result.append(
          l10n_util::GetStringUTF16(IDS_TOOLTIP_TAB_ALERT_STATE_USB_CONNECTED));
      break;
    case TabAlertState::NONE:
      NOTREACHED();
      break;
  }
  return result;
}
