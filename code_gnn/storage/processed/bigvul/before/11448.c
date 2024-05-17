fbFetch_b1g2r1 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  p = Fetch4(bits, i + x);
        CARD32  r,g,b;

        b = ((p & 0x8) * 0xff) >> 3;
        g = ((p & 0x6) * 0x55) << 7;
        r = ((p & 0x1) * 0xff) << 16;
        WRITE(buffer++, 0xff000000|r|g|b);
    }
}
