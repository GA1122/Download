fbFetch_r5g6b5 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD16 *pixel = (const CARD16 *)bits + x;
    const CARD16 *end = pixel + width;
    while (pixel < end) {
        CARD32  p = READ(pixel++);
        CARD32 r = (((p) << 3) & 0xf8) | 
	    (((p) << 5) & 0xfc00) |
	    (((p) << 8) & 0xf80000);
        r |= (r >> 5) & 0x70007;
        r |= (r >> 6) & 0x300;
        WRITE(buffer++, 0xff000000 | r);
    }
}
