void WebsiteSettingsPopupAndroid::SetPermissionInfo(
    const PermissionInfoList& permission_info_list) {
  JNIEnv* env = base::android::AttachCurrentThread();

  std::vector<ContentSettingsType> permissions_to_display;
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_GEOLOCATION);
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA);
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC);
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_NOTIFICATIONS);
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_IMAGES);
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_JAVASCRIPT);
  permissions_to_display.push_back(CONTENT_SETTINGS_TYPE_POPUPS);

  std::map<ContentSettingsType, ContentSetting>
      user_specified_settings_to_display;

  for (const auto& permission : permission_info_list) {
    if (std::find(permissions_to_display.begin(),
                  permissions_to_display.end(),
                  permission.type) != permissions_to_display.end() &&
        permission.setting != CONTENT_SETTING_DEFAULT) {
      user_specified_settings_to_display[permission.type] = permission.setting;
    }
  }

  for (const auto& permission : permissions_to_display) {
    if (ContainsKey(user_specified_settings_to_display, permission)) {
      base::string16 setting_title =
          WebsiteSettingsUI::PermissionTypeToUIString(permission);

      Java_WebsiteSettingsPopup_addPermissionSection(
          env,
          popup_jobject_.obj(),
          ConvertUTF16ToJavaString(env, setting_title).obj(),
          static_cast<jint>(permission),
          static_cast<jint>(user_specified_settings_to_display[permission]));
    }
  }

  Java_WebsiteSettingsPopup_updatePermissionDisplay(env, popup_jobject_.obj());
}
