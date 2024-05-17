static jlong Region_constructor(JNIEnv* env, jobject) {
 return reinterpret_cast<jlong>(new SkRegion);
}
