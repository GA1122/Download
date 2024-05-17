const gfx::ImageSkia PageInfoUI::GetPermissionIcon(const PermissionInfo& info,
                                                   SkColor related_text_color) {
  const gfx::VectorIcon* icon = &gfx::kNoneIcon;
  switch (info.type) {
    case CONTENT_SETTINGS_TYPE_COOKIES:
      icon = &kCookieIcon;
      break;
    case CONTENT_SETTINGS_TYPE_IMAGES:
      icon = &kPhotoIcon;
      break;
    case CONTENT_SETTINGS_TYPE_JAVASCRIPT:
      icon = &kCodeIcon;
      break;
    case CONTENT_SETTINGS_TYPE_POPUPS:
      icon = &kLaunchIcon;
      break;
#if BUILDFLAG(ENABLE_PLUGINS)
    case CONTENT_SETTINGS_TYPE_PLUGINS:
      icon = &kExtensionIcon;
      break;
#endif
    case CONTENT_SETTINGS_TYPE_GEOLOCATION:
      icon = &vector_icons::kLocationOnIcon;
      break;
    case CONTENT_SETTINGS_TYPE_NOTIFICATIONS:
      icon = &vector_icons::kNotificationsIcon;
      break;
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC:
      icon = &vector_icons::kMicIcon;
      break;
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA:
      icon = &vector_icons::kVideocamIcon;
      break;
    case CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS:
      icon = &kFileDownloadIcon;
      break;
    case CONTENT_SETTINGS_TYPE_MIDI_SYSEX:
      icon = &vector_icons::kMidiIcon;
      break;
    case CONTENT_SETTINGS_TYPE_BACKGROUND_SYNC:
      icon = &kSyncIcon;
      break;
    case CONTENT_SETTINGS_TYPE_ADS:
      icon = &kAdsIcon;
      break;
    case CONTENT_SETTINGS_TYPE_SOUND:
      icon = &kVolumeUpIcon;
      break;
    case CONTENT_SETTINGS_TYPE_CLIPBOARD_READ:
      icon = &kPageInfoContentPasteIcon;
      break;
    case CONTENT_SETTINGS_TYPE_SENSORS:
      icon = &kSensorsIcon;
      break;
    case CONTENT_SETTINGS_TYPE_USB_GUARD:
      icon = &vector_icons::kUsbIcon;
      break;
#if !defined(OS_ANDROID)
    case CONTENT_SETTINGS_TYPE_SERIAL_GUARD:
      icon = &vector_icons::kSerialPortIcon;
      break;
    case CONTENT_SETTINGS_TYPE_BLUETOOTH_SCANNING:
      icon = &vector_icons::kBluetoothScanningIcon;
      break;
#endif
    default:
      NOTREACHED();
      break;
  }

  ContentSetting setting = info.setting == CONTENT_SETTING_DEFAULT
                               ? info.default_setting
                               : info.setting;
  if (setting == CONTENT_SETTING_BLOCK) {
    return gfx::CreateVectorIconWithBadge(
        *icon, kVectorIconSize,
        color_utils::DeriveDefaultIconColor(related_text_color),
        kBlockedBadgeIcon);
  }
  return gfx::CreateVectorIcon(
      *icon, kVectorIconSize,
      color_utils::DeriveDefaultIconColor(related_text_color));
}
