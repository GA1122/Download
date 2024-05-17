static jint Bitmap_getPixel(JNIEnv* env, jobject, jlong bitmapHandle,
        jint x, jint y) {
 const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 SkAutoLockPixels alp(*bitmap);

 ToColorProc proc = ChooseToColorProc(*bitmap);
 if (NULL == proc) {
 return 0;
 }
 const void* src = bitmap->getAddr(x, y);
 if (NULL == src) {
 return 0;
 }

 SkColor dst[1];
    proc(dst, src, 1, bitmap->getColorTable());
 return static_cast<jint>(dst[0]);
}
