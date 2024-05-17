static void Bitmap_erase(JNIEnv* env, jobject, jlong bitmapHandle, jint color) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
    bitmap->eraseColor(color);
}
