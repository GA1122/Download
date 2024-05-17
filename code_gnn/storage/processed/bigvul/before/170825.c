static void FromColor_D32_Raw(void* dst, const SkColor src[], int width,
 int, int) {
 if (SK_COLOR_MATCHES_PMCOLOR_BYTE_ORDER) {
        memcpy(dst, src, width * sizeof(SkColor));
 return;
 }

 SkPMColor* d = (SkPMColor*)dst;
 for (int i = 0; i < width; i++) {
 SkColor c = *src++;
 *d++ = SkPackARGB32NoCheck(SkColorGetA(c), SkColorGetR(c),
 SkColorGetG(c), SkColorGetB(c));
 }
}
