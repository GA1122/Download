static void JNI_WebApkUpdateManager_UpdateWebApkFromFile(
    JNIEnv* env,
    const JavaParamRef<jstring>& java_update_request_path,
    const JavaParamRef<jobject>& java_callback) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  ScopedJavaGlobalRef<jobject> callback_ref(java_callback);

  Profile* profile = ProfileManager::GetLastUsedProfile();
  if (profile == nullptr) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::Bind(&OnUpdated, callback_ref, WebApkInstallResult::FAILURE,
                   false  , ""  ));
    return;
  }

  std::string update_request_path =
      ConvertJavaStringToUTF8(env, java_update_request_path);
  WebApkInstallService::Get(profile)->UpdateAsync(
      base::FilePath(update_request_path),
      base::Bind(&OnUpdated, callback_ref));
}
