void DownloadController::Init(JNIEnv* env, jobject obj) {
  java_object_ = new JavaObject;
  java_object_->obj_ = env->NewWeakGlobalRef(obj);
}
