static void ToColor_S4444_Alpha(SkColor dst[], const void* src, int width,
 SkColorTable*) {
 SkASSERT(width > 0);
 const SkPMColor16* s = (const SkPMColor16*)src;
 do {
 *dst++ = SkUnPreMultiply::PMColorToColor(SkPixel4444ToPixel32(*s++));
 } while (--width != 0);
}
