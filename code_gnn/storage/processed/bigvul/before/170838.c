static void ToColor_SI8_Raw(SkColor dst[], const void* src, int width,
 SkColorTable* ctable) {
 SkASSERT(width > 0);
 const uint8_t* s = (const uint8_t*)src;
 const SkPMColor* colors = ctable->lockColors();
 do {
 SkPMColor c = colors[*s++];
 *dst++ = SkColorSetARGB(SkGetPackedA32(c), SkGetPackedR32(c),
 SkGetPackedG32(c), SkGetPackedB32(c));
 } while (--width != 0);
    ctable->unlockColors();
}
