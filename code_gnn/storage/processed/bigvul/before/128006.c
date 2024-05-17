jint GetNativeInstanceCount(JNIEnv* env, const JavaParamRef<jclass>&) {
  return base::subtle::NoBarrier_Load(&g_instance_count);
}
