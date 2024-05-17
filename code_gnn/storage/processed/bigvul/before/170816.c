static jboolean Bitmap_sameAs(JNIEnv* env, jobject, jlong bm0Handle,
                              jlong bm1Handle) {
 const SkBitmap* bm0 = reinterpret_cast<SkBitmap*>(bm0Handle);
 const SkBitmap* bm1 = reinterpret_cast<SkBitmap*>(bm1Handle);
 if (bm0->width() != bm1->width() ||
        bm0->height() != bm1->height() ||
        bm0->colorType() != bm1->colorType()) {
 return JNI_FALSE;
 }

 SkAutoLockPixels alp0(*bm0);
 SkAutoLockPixels alp1(*bm1);

 if (NULL == bm0->getPixels() || NULL == bm1->getPixels()) {
 return JNI_FALSE;
 }

 if (bm0->colorType() == kIndex_8_SkColorType) {
 SkColorTable* ct0 = bm0->getColorTable();
 SkColorTable* ct1 = bm1->getColorTable();
 if (NULL == ct0 || NULL == ct1) {
 return JNI_FALSE;
 }
 if (ct0->count() != ct1->count()) {
 return JNI_FALSE;
 }

 SkAutoLockColors alc0(ct0);
 SkAutoLockColors alc1(ct1);
 const size_t size = ct0->count() * sizeof(SkPMColor);
 if (memcmp(alc0.colors(), alc1.colors(), size) != 0) {
 return JNI_FALSE;
 }
 }

 const int h = bm0->height();
 const size_t size = bm0->width() * bm0->bytesPerPixel();
 for (int y = 0; y < h; y++) {
 void *bm0Addr = bm0->getAddr(0, y);
 void *bm1Addr = bm1->getAddr(0, y);

 if(bm0Addr == NULL || bm1Addr == NULL) {
 return JNI_FALSE;
 }

 if (memcmp(bm0Addr, bm1Addr, size) != 0) {
 return JNI_FALSE;
 }
 }
 return JNI_TRUE;
}
