static jobject Bitmap_copy(JNIEnv* env, jobject, jlong srcHandle,
                           jint dstConfigHandle, jboolean isMutable) {
 const SkBitmap* src = reinterpret_cast<SkBitmap*>(srcHandle);
 SkColorType dstCT = GraphicsJNI::legacyBitmapConfigToColorType(dstConfigHandle);
 SkBitmap            result;
 JavaPixelAllocator  allocator(env);

 if (!src->copyTo(&result, dstCT, &allocator)) {
 return NULL;
 }
 return GraphicsJNI::createBitmap(env, new SkBitmap(result), allocator.getStorageObj(),
            getPremulBitmapCreateFlags(isMutable), NULL, NULL);
}
