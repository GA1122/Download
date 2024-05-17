static jboolean Region_getBoundaryPath(JNIEnv* env, jobject, jlong regionHandle, jlong pathHandle) {
 const SkRegion* region = reinterpret_cast<SkRegion*>(regionHandle);
 SkPath*   path = reinterpret_cast<SkPath*>(pathHandle);
 bool result = region->getBoundaryPath(path);
 return boolTojboolean(result);
}
