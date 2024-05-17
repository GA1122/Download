bool MediaStreamDevicesController::IsRequestBlockedByDefault() const {
  if (microphone_requested_ &&
      profile_->GetHostContentSettingsMap()->GetContentSetting(
          request_.security_origin,
          request_.security_origin,
          CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC,
          NO_RESOURCE_IDENTIFIER) != CONTENT_SETTING_BLOCK) {
    return false;
  }

  if (webcam_requested_ &&
      profile_->GetHostContentSettingsMap()->GetContentSetting(
          request_.security_origin,
          request_.security_origin,
          CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA,
          NO_RESOURCE_IDENTIFIER) != CONTENT_SETTING_BLOCK) {
    return false;
  }

  return true;
}
