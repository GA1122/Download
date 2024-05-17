 static jboolean Bitmap_writeToParcel(JNIEnv* env, jobject,
                                     jlong bitmapHandle,
                                     jboolean isMutable, jint density,
                                     jobject parcel) {
 const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 if (parcel == NULL) {
 SkDebugf("------- writeToParcel null parcel\n");
 return JNI_FALSE;
 }

    android::Parcel* p = android::parcelForJavaObject(env, parcel);

    p->writeInt32(isMutable);
    p->writeInt32(bitmap->colorType());
    p->writeInt32(bitmap->alphaType());
    p->writeInt32(bitmap->width());
    p->writeInt32(bitmap->height());
    p->writeInt32(bitmap->rowBytes());
    p->writeInt32(density);

 if (bitmap->colorType() == kIndex_8_SkColorType) {
 SkColorTable* ctable = bitmap->getColorTable();
 if (ctable != NULL) {
 int count = ctable->count();
            p->writeInt32(count);
            memcpy(p->writeInplace(count * sizeof(SkPMColor)),
                   ctable->lockColors(), count * sizeof(SkPMColor));
            ctable->unlockColors();
 } else {
            p->writeInt32(0);  
 }
 }

 size_t size = bitmap->getSize();

    android::Parcel::WritableBlob blob;
    android::status_t status = p->writeBlob(size, &blob);
 if (status) {
        doThrowRE(env, "Could not write bitmap to parcel blob.");
 return JNI_FALSE;
 }

    bitmap->lockPixels();
 const void* pSrc =  bitmap->getPixels();
 if (pSrc == NULL) {
        memset(blob.data(), 0, size);
 } else {
        memcpy(blob.data(), pSrc, size);
 }
    bitmap->unlockPixels();

    blob.release();
 return JNI_TRUE;
}
