static jboolean Bitmap_hasAlpha(JNIEnv* env, jobject, jlong bitmapHandle) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 return !bitmap->isOpaque() ? JNI_TRUE : JNI_FALSE;
}
