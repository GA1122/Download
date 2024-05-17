static void ToColor_SI8_Alpha(SkColor dst[], const void* src, int width,
 SkColorTable* ctable) {
 SkASSERT(width > 0);
 const uint8_t* s = (const uint8_t*)src;
 const SkPMColor* colors = ctable->lockColors();
 do {
 *dst++ = SkUnPreMultiply::PMColorToColor(colors[*s++]);
 } while (--width != 0);
    ctable->unlockColors();
}
