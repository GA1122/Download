static jboolean Bitmap_isPremultiplied(JNIEnv* env, jobject, jlong bitmapHandle) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 if (bitmap->alphaType() == kPremul_SkAlphaType) {
 return JNI_TRUE;
 }
 return JNI_FALSE;
}
