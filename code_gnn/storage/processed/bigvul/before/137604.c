void ChromeDownloadDelegate::RequestFileAccess(intptr_t callback_id) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_ChromeDownloadDelegate_requestFileAccess(
      env, java_ref_, callback_id);
}
