static void Bitmap_copyPixelsFromBuffer(JNIEnv* env, jobject,
                                        jlong bitmapHandle, jobject jbuffer) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 SkAutoLockPixels alp(*bitmap);
 void* dst = bitmap->getPixels();

 if (NULL != dst) {
        android::AutoBufferPointer abp(env, jbuffer, JNI_FALSE);
        memcpy(dst, abp.pointer(), bitmap->getSize());
        bitmap->notifyPixelsChanged();
 }
}
