static void WriteSyncsafeInteger(uint8_t *dst, size_t x) {
 for (size_t i = 0; i < 4; ++i) {
        dst[3 - i] = (x & 0x7f);
        x >>= 7;
 }
}
