static void Bitmap_setHasMipMap(JNIEnv* env, jobject, jlong bitmapHandle,
                                jboolean hasMipMap) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
    bitmap->setHasHardwareMipMap(hasMipMap);
}
