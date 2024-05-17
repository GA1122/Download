bool DownloadController::HasFileAccessPermission(
    ui::WindowAndroid* window_android) {
  ScopedJavaLocalRef<jobject> jwindow_android = window_android->GetJavaObject();

  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!jwindow_android.is_null());

  JNIEnv* env = base::android::AttachCurrentThread();
  return Java_DownloadController_hasFileAccess(
       env, GetJavaObject()->Controller(env), jwindow_android);
 }
