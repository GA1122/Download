static jboolean JNI_ChromeFeatureList_IsInitialized(JNIEnv* env) {
  return !!base::FeatureList::GetInstance();
}
