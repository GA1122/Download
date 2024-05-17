 void ForeignSessionHelper::SetOnForeignSessionCallback(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    const JavaParamRef<jobject>& callback) {
  callback_.Reset(env, callback);
}
