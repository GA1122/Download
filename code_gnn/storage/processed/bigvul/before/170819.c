static void Bitmap_setPixels(JNIEnv* env, jobject, jlong bitmapHandle,
        jintArray pixelArray, jint offset, jint stride,
        jint x, jint y, jint width, jint height) {
 const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 GraphicsJNI::SetPixels(env, pixelArray, offset, stride,
            x, y, width, height, *bitmap);
}
