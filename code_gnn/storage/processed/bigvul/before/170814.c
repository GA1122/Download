static void Bitmap_reconfigure(JNIEnv* env, jobject clazz, jlong bitmapHandle,
        jint width, jint height, jint configHandle, jint allocSize,
        jboolean requestPremul) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 SkColorType colorType = GraphicsJNI::legacyBitmapConfigToColorType(configHandle);

 if (colorType == kARGB_4444_SkColorType) {
        colorType = kN32_SkColorType;
 }

 if (width * height * SkColorTypeBytesPerPixel(colorType) > allocSize) {
        doThrowIAE(env, "Bitmap not large enough to support new configuration");
 return;
 }
 SkPixelRef* ref = bitmap->pixelRef();
    ref->ref();
 SkAlphaType alphaType;
 if (bitmap->colorType() != kRGB_565_SkColorType
 && bitmap->alphaType() == kOpaque_SkAlphaType) {
        alphaType = kOpaque_SkAlphaType;
 } else {
        alphaType = requestPremul ? kPremul_SkAlphaType : kUnpremul_SkAlphaType;
 }
    bitmap->setInfo(SkImageInfo::Make(width, height, colorType, alphaType));
 SkImageInfo& info = const_cast<SkImageInfo&>(ref->info());
    info = bitmap->info();
    bitmap->setPixelRef(ref);

    ref->notifyPixelsChanged();
    ref->unref();
}
