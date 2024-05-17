static jboolean Region_isRect(JNIEnv* env, jobject region) {
 bool result = GetSkRegion(env, region)->isRect();
 return boolTojboolean(result);
}
