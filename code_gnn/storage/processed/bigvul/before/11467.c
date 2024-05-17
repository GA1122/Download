fbStore_a1 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  *pixel = ((CARD32 *) bits) + ((i+x) >> 5);
        CARD32  mask = FbStipMask((i+x) & 0x1f, 1);

        CARD32 v = READ(values + i) & 0x80000000 ? mask : 0;
        WRITE(pixel, (READ(pixel) & ~mask) | v);
    }
}
