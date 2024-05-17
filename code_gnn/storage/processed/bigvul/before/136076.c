void WebsiteSettingsPopupAndroid::OnPermissionSettingChanged(JNIEnv* env,
                                                             jobject obj,
                                                             jint type,
                                                             jint setting) {
  ContentSettingsType content_setting_type =
      static_cast<ContentSettingsType>(type);
  ContentSetting content_setting = static_cast<ContentSetting>(setting);
  presenter_->OnSitePermissionChanged(content_setting_type, content_setting);
}
