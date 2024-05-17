static jboolean Region_getBounds(JNIEnv* env, jobject, jlong regionHandle, jobject rectBounds) {
 SkRegion* region = reinterpret_cast<SkRegion*>(regionHandle);
 GraphicsJNI::irect_to_jrect(region->getBounds(), env, rectBounds);
 bool result = !region->isEmpty();
 return boolTojboolean(result);
}
