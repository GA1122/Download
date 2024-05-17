static jboolean Region_setRect(JNIEnv* env, jobject, jlong dstHandle, jint left, jint top, jint right, jint bottom) {
 SkRegion* dst = reinterpret_cast<SkRegion*>(dstHandle);
 bool result = dst->setRect(left, top, right, bottom);
 return boolTojboolean(result);
}
