static void FromColor_D4444_Raw(void* dst, const SkColor src[], int width,
 int x, int y) {
 SkPMColor16* d = (SkPMColor16*)dst;

    DITHER_4444_SCAN(y);
 for (int stop = x + width; x < stop; x++) {
 SkColor c = *src++;

 SkPMColor pmc = SkPackARGB32NoCheck(SkColorGetA(c), SkColorGetR(c),
 SkColorGetG(c), SkColorGetB(c));
 *d++ = SkDitherARGB32To4444(pmc, DITHER_VALUE(x));
 }
}
