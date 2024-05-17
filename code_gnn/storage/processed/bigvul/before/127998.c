static jlong GetAwDrawGLFunction(JNIEnv* env, const JavaParamRef<jclass>&) {
  return reinterpret_cast<intptr_t>(&DrawGLFunction);
}
