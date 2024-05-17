static void ToColor_S32_Alpha(SkColor dst[], const void* src, int width,
 SkColorTable*) {
 SkASSERT(width > 0);
 const SkPMColor* s = (const SkPMColor*)src;
 do {
 *dst++ = SkUnPreMultiply::PMColorToColor(*s++);
 } while (--width != 0);
}
