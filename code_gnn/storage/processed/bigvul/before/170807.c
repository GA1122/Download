static jint Bitmap_getGenerationId(JNIEnv* env, jobject, jlong bitmapHandle) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 return static_cast<jint>(bitmap->getGenerationID());
}
