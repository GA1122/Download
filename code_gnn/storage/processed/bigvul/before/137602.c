 void ChromeDownloadDelegate::OnDangerousDownload(const std::string& filename,
                                                 const std::string& guid) {
  JNIEnv* env = base::android::AttachCurrentThread();
  ScopedJavaLocalRef<jstring> jfilename = ConvertUTF8ToJavaString(
      env, filename);
  ScopedJavaLocalRef<jstring> jguid = ConvertUTF8ToJavaString(env, guid);
  Java_ChromeDownloadDelegate_onDangerousDownload(env, java_ref_, jfilename,
                                                  jguid);
}
