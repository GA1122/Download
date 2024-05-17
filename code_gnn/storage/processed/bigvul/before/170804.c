static jobject Bitmap_creator(JNIEnv* env, jobject, jintArray jColors,
                              jint offset, jint stride, jint width, jint height,
                              jint configHandle, jboolean isMutable) {
 SkColorType colorType = GraphicsJNI::legacyBitmapConfigToColorType(configHandle);
 if (NULL != jColors) {
 size_t n = env->GetArrayLength(jColors);
 if (n < SkAbs32(stride) * (size_t)height) {
            doThrowAIOOBE(env);
 return NULL;
 }
 }

 if (colorType == kARGB_4444_SkColorType) {
        colorType = kN32_SkColorType;
 }

 SkBitmap bitmap;
    bitmap.setInfo(SkImageInfo::Make(width, height, colorType, kPremul_SkAlphaType));

    jbyteArray buff = GraphicsJNI::allocateJavaPixelRef(env, &bitmap, NULL);
 if (NULL == buff) {
 return NULL;
 }

 if (jColors != NULL) {
 GraphicsJNI::SetPixels(env, jColors, offset, stride,
 0, 0, width, height, bitmap);
 }

 return GraphicsJNI::createBitmap(env, new SkBitmap(bitmap), buff,
            getPremulBitmapCreateFlags(isMutable), NULL, NULL);
}
