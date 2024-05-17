std::string PermissionUtil::ConvertContentSettingsTypeToSafeBrowsingName(
    ContentSettingsType permission_type) {
  switch (permission_type) {
    case CONTENT_SETTINGS_TYPE_GEOLOCATION:
      return "GEOLOCATION";
    case CONTENT_SETTINGS_TYPE_NOTIFICATIONS:
      return "NOTIFICATIONS";
    case CONTENT_SETTINGS_TYPE_MIDI_SYSEX:
      return "MIDI_SYSEX";
    case CONTENT_SETTINGS_TYPE_PUSH_MESSAGING:
      return "PUSH_MESSAGING";
    case CONTENT_SETTINGS_TYPE_DURABLE_STORAGE:
      return "DURABLE_STORAGE";
    case CONTENT_SETTINGS_TYPE_PROTECTED_MEDIA_IDENTIFIER:
      return "PROTECTED_MEDIA_IDENTIFIER";
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC:
      return "AUDIO_CAPTURE";
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA:
      return "VIDEO_CAPTURE";
    case CONTENT_SETTINGS_TYPE_BACKGROUND_SYNC:
      return "BACKGROUND_SYNC";
    case CONTENT_SETTINGS_TYPE_PLUGINS:
      return "FLASH";
    default:
      break;
  }
  NOTREACHED();
  return std::string();
}
