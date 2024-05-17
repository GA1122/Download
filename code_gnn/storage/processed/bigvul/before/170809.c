static void Bitmap_getPixels(JNIEnv* env, jobject, jlong bitmapHandle,
        jintArray pixelArray, jint offset, jint stride,
        jint x, jint y, jint width, jint height) {
 const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 SkAutoLockPixels alp(*bitmap);

 ToColorProc proc = ChooseToColorProc(*bitmap);
 if (NULL == proc) {
 return;
 }
 const void* src = bitmap->getAddr(x, y);
 if (NULL == src) {
 return;
 }

 SkColorTable* ctable = bitmap->getColorTable();
    jint* dst = env->GetIntArrayElements(pixelArray, NULL);
 SkColor* d = (SkColor*)dst + offset;
 while (--height >= 0) {
        proc(d, src, width, ctable);
        d += stride;
        src = (void*)((const char*)src + bitmap->rowBytes());
 }
    env->ReleaseIntArrayElements(pixelArray, dst, 0);
}
