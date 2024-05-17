static jint Bitmap_config(JNIEnv* env, jobject, jlong bitmapHandle) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 return GraphicsJNI::colorTypeToLegacyBitmapConfig(bitmap->colorType());
}
