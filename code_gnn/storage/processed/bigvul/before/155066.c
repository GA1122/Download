static ScopedJavaLocalRef<jobject> JNI_SendTabToSelfAndroidBridge_AddEntry(
    JNIEnv* env,
    const JavaParamRef<jobject>& j_profile,
    const JavaParamRef<jstring>& j_url,
    const JavaParamRef<jstring>& j_title,
    jlong j_navigation_time,
    const JavaParamRef<jstring>& j_target_device_sync_cache_guid) {
  const std::string url = ConvertJavaStringToUTF8(env, j_url);
  const std::string title = ConvertJavaStringToUTF8(env, j_title);
  const std::string target_device_sync_cache_guid =
      ConvertJavaStringToUTF8(env, j_target_device_sync_cache_guid);
  base::Time navigation_time = base::Time::FromJavaTime(j_navigation_time);

  SendTabToSelfModel* model = GetModel(j_profile);
  if (!model->IsReady()) {
    return nullptr;
  }

  const SendTabToSelfEntry* persisted_entry = model->AddEntry(
      GURL(url), title, navigation_time, target_device_sync_cache_guid);

  if (persisted_entry == nullptr) {
    return nullptr;
  }

  return CreateJavaSendTabToSelfEntry(env, persisted_entry);
}
