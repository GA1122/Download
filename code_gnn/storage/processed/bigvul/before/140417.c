PermissionRequestType PermissionUtil::GetRequestType(ContentSettingsType type) {
  switch (type) {
    case CONTENT_SETTINGS_TYPE_GEOLOCATION:
      return PermissionRequestType::PERMISSION_GEOLOCATION;
    case CONTENT_SETTINGS_TYPE_NOTIFICATIONS:
      return PermissionRequestType::PERMISSION_NOTIFICATIONS;
    case CONTENT_SETTINGS_TYPE_MIDI_SYSEX:
      return PermissionRequestType::PERMISSION_MIDI_SYSEX;
    case CONTENT_SETTINGS_TYPE_PUSH_MESSAGING:
      return PermissionRequestType::PERMISSION_PUSH_MESSAGING;
    case CONTENT_SETTINGS_TYPE_PROTECTED_MEDIA_IDENTIFIER:
      return PermissionRequestType::PERMISSION_PROTECTED_MEDIA_IDENTIFIER;
    case CONTENT_SETTINGS_TYPE_PLUGINS:
      return PermissionRequestType::PERMISSION_FLASH;
    default:
      NOTREACHED();
      return PermissionRequestType::UNKNOWN;
  }
}
