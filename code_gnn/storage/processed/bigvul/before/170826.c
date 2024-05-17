static void FromColor_D4444(void* dst, const SkColor src[], int width,
 int x, int y) {
 SkPMColor16* d = (SkPMColor16*)dst;

    DITHER_4444_SCAN(y);
 for (int stop = x + width; x < stop; x++) {
 SkPMColor pmc = SkPreMultiplyColor(*src++);
 *d++ = SkDitherARGB32To4444(pmc, DITHER_VALUE(x));
 }
}
