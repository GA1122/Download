static jboolean Region_quickRejectRgn(JNIEnv* env, jobject region, jobject other) {
 bool result = GetSkRegion(env, region)->quickReject(*GetSkRegion(env, other));
 return boolTojboolean(result);
}
