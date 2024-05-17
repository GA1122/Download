static void ToColor_S565(SkColor dst[], const void* src, int width,
 SkColorTable*) {
 SkASSERT(width > 0);
 const uint16_t* s = (const uint16_t*)src;
 do {
 uint16_t c = *s++;
 *dst++ = SkColorSetRGB(SkPacked16ToR32(c), SkPacked16ToG32(c),
 SkPacked16ToB32(c));
 } while (--width != 0);
}
