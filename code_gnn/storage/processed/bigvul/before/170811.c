static jboolean Bitmap_hasMipMap(JNIEnv* env, jobject, jlong bitmapHandle) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 return bitmap->hasHardwareMipMap() ? JNI_TRUE : JNI_FALSE;
}
