static void Bitmap_setHasAlpha(JNIEnv* env, jobject, jlong bitmapHandle,
        jboolean hasAlpha, jboolean requestPremul) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 if (hasAlpha) {
        bitmap->setAlphaType(requestPremul ? kPremul_SkAlphaType : kUnpremul_SkAlphaType);
 } else {
        bitmap->setAlphaType(kOpaque_SkAlphaType);
 }
}
