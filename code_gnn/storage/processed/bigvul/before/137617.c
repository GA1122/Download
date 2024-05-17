static void Init(JNIEnv* env, const JavaParamRef<jobject>& obj) {
  DownloadController::GetInstance()->Init(env, obj);
}
