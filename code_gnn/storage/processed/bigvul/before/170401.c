static jboolean Region_isComplex(JNIEnv* env, jobject region) {
 bool result = GetSkRegion(env, region)->isComplex();
 return boolTojboolean(result);
}
