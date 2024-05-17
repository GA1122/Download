void WebsiteSettingsPopupAndroid::SetIdentityInfo(
    const IdentityInfo& identity_info) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_WebsiteSettingsPopup_showDialog(env, popup_jobject_.obj());
}
