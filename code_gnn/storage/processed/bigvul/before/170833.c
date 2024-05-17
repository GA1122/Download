static void ToColor_S4444_Opaque(SkColor dst[], const void* src, int width,
 SkColorTable*) {
 SkASSERT(width > 0);
 const SkPMColor16* s = (const SkPMColor16*)src;
 do {
 SkPMColor c = SkPixel4444ToPixel32(*s++);
 *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
 SkGetPackedB32(c));
 } while (--width != 0);
}
