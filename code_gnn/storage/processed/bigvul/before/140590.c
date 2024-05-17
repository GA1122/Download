static jboolean JNI_ChromeFeatureList_IsInitialized(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz) {
  return !!base::FeatureList::GetInstance();
}
