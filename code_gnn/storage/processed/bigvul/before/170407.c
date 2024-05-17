static jboolean Region_quickContains(JNIEnv* env, jobject region, jint left, jint top, jint right, jint bottom) {
 bool result = GetSkRegion(env, region)->quickContains(left, top, right, bottom);
 return boolTojboolean(result);
}
