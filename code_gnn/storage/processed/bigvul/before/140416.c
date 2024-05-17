std::string PermissionUtil::GetPermissionString(
    ContentSettingsType content_type) {
  switch (content_type) {
    case CONTENT_SETTINGS_TYPE_GEOLOCATION:
      return "Geolocation";
    case CONTENT_SETTINGS_TYPE_NOTIFICATIONS:
      return "Notifications";
    case CONTENT_SETTINGS_TYPE_MIDI_SYSEX:
      return "MidiSysEx";
    case CONTENT_SETTINGS_TYPE_PUSH_MESSAGING:
      return "PushMessaging";
    case CONTENT_SETTINGS_TYPE_DURABLE_STORAGE:
      return "DurableStorage";
    case CONTENT_SETTINGS_TYPE_PROTECTED_MEDIA_IDENTIFIER:
      return "ProtectedMediaIdentifier";
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC:
      return "AudioCapture";
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA:
      return "VideoCapture";
    case CONTENT_SETTINGS_TYPE_MIDI:
      return "Midi";
    case CONTENT_SETTINGS_TYPE_BACKGROUND_SYNC:
      return "BackgroundSync";
    case CONTENT_SETTINGS_TYPE_PLUGINS:
      return "Flash";
    default:
      break;
  }
  NOTREACHED();
  return std::string();
}
