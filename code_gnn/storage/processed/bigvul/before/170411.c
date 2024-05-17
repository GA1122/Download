static jboolean Region_setPath(JNIEnv* env, jobject, jlong dstHandle,
                               jlong pathHandle, jlong clipHandle) {
 SkRegion*       dst  = reinterpret_cast<SkRegion*>(dstHandle);
 const SkPath*   path = reinterpret_cast<SkPath*>(pathHandle);
 const SkRegion* clip = reinterpret_cast<SkRegion*>(clipHandle);
 SkASSERT(dst && path && clip);
 bool result = dst->setPath(*path, *clip);
 return boolTojboolean(result);

}
