const std::string GetRapporMetric(ContentSettingsType permission) {
  std::string permission_str;
  switch (permission) {
    case CONTENT_SETTINGS_TYPE_GEOLOCATION:
      permission_str = "Geolocation";
      break;
    case CONTENT_SETTINGS_TYPE_NOTIFICATIONS:
      permission_str = "Notifications";
      break;
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC:
      permission_str = "Mic";
      break;
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA:
      permission_str = "Camera";
      break;
    default:
      return "";
  }

  return base::StringPrintf("ContentSettings.PermissionActions_%s.Revoked.Url",
                            permission_str.c_str());
}
