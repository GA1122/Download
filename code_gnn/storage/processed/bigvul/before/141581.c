static jlong JNI_ForeignSessionHelper_Init(
    JNIEnv* env,
    const JavaParamRef<jobject>& profile) {
  ForeignSessionHelper* foreign_session_helper = new ForeignSessionHelper(
      ProfileAndroid::FromProfileAndroid(profile));
  return reinterpret_cast<intptr_t>(foreign_session_helper);
}
