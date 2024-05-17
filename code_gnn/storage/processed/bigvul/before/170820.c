static void Bitmap_setPremultiplied(JNIEnv* env, jobject, jlong bitmapHandle,
        jboolean isPremul) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 if (!bitmap->isOpaque()) {
 if (isPremul) {
            bitmap->setAlphaType(kPremul_SkAlphaType);
 } else {
            bitmap->setAlphaType(kUnpremul_SkAlphaType);
 }
 }
}
