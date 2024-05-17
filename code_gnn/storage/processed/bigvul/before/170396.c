static jboolean Region_contains(JNIEnv* env, jobject region, jint x, jint y) {
 bool result = GetSkRegion(env, region)->contains(x, y);
 return boolTojboolean(result);
}
