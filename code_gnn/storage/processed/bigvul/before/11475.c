fbStore_a4r4g4b4 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD16  *pixel = ((CARD16 *) bits) + x;
    for (i = 0; i < width; ++i) {
        Splita(READ(values + i));
        WRITE(pixel++, ((a << 8) & 0xf000) |
	      ((r << 4) & 0x0f00) |
	      ((g     ) & 0x00f0) |
	      ((b >> 4)         ));
    }
}
