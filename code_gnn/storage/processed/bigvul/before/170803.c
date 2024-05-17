static void Bitmap_copyPixelsToBuffer(JNIEnv* env, jobject,
                                      jlong bitmapHandle, jobject jbuffer) {
 const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 SkAutoLockPixels alp(*bitmap);
 const void* src = bitmap->getPixels();

 if (NULL != src) {
        android::AutoBufferPointer abp(env, jbuffer, JNI_TRUE);

        memcpy(abp.pointer(), src, bitmap->getSize());
 }
}
