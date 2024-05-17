bool GraphicsJNI::SetPixels(JNIEnv* env, jintArray srcColors, int srcOffset, int srcStride,
 int x, int y, int width, int height, const SkBitmap& dstBitmap) {
 SkAutoLockPixels alp(dstBitmap);
 void* dst = dstBitmap.getPixels();
 FromColorProc proc = ChooseFromColorProc(dstBitmap);

 if (NULL == dst || NULL == proc) {
 return false;
 }

 const jint* array = env->GetIntArrayElements(srcColors, NULL);
 const SkColor* src = (const SkColor*)array + srcOffset;

    dst = dstBitmap.getAddr(x, y);
 for (int y = 0; y < height; y++) {
        proc(dst, src, width, x, y);
        src += srcStride;
        dst = (char*)dst + dstBitmap.rowBytes();
 }

    dstBitmap.notifyPixelsChanged();

    env->ReleaseIntArrayElements(srcColors, const_cast<jint*>(array),
                                 JNI_ABORT);
 return true;
}
