    DownloadController::GetJavaObject() {
  if (!java_object_) {
    JNIEnv* env = base::android::AttachCurrentThread();
    Java_DownloadController_getInstance(env);
  }

  DCHECK(java_object_);
  return java_object_;
}
