static void Region_setRegion(JNIEnv* env, jobject, jlong dstHandle, jlong srcHandle) {
 SkRegion* dst = reinterpret_cast<SkRegion*>(dstHandle);
 const SkRegion* src = reinterpret_cast<SkRegion*>(srcHandle);
 SkASSERT(dst && src);
 *dst = *src;
}
