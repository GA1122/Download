static void LaunchPermissionUpdateInfoBar(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jobject>& tab,
    const JavaParamRef<jstring>& jpermission,
    jlong callback_id) {
  TabAndroid* tab_android = TabAndroid::GetNativeTab(env, tab);

  std::string permission =
      base::android::ConvertJavaStringToUTF8(env, jpermission);

  static_assert(
      std::is_same<
          DownloadControllerBase::AcquireFileAccessPermissionCallback,
          base::Callback<void(bool)>>::value,
      "Callback types don't match!");
  std::unique_ptr<base::Callback<void(bool)>> cb(
      reinterpret_cast<base::Callback<void(bool)>*>(callback_id));

  std::vector<std::string> permissions;
  permissions.push_back(permission);

  PermissionUpdateInfoBarDelegate::Create(
      tab_android->web_contents(),
      permissions,
      IDS_MISSING_STORAGE_PERMISSION_DOWNLOAD_EDUCATION_TEXT,
      *cb);
}
