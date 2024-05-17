static void DangerousDownloadValidated(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jobject>& tab,
    const JavaParamRef<jstring>& jdownload_guid,
    jboolean accept) {
  std::string download_guid =
      base::android::ConvertJavaStringToUTF8(env, jdownload_guid);
  TabAndroid* tab_android = TabAndroid::GetNativeTab(env, tab);
  DownloadControllerBase::Get()->DangerousDownloadValidated(
      tab_android->web_contents(), download_guid, accept);
}
