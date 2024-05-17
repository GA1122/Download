fbFetchPixel_a1b1g1r1 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = Fetch4(bits, offset);
    CARD32  a,r,g,b;

    a = ((pixel & 0x8) * 0xff) << 21;
    r = ((pixel & 0x4) * 0xff) >> 3;
    g = ((pixel & 0x2) * 0xff) << 7;
    b = ((pixel & 0x1) * 0xff) << 16;
    return a|r|g|b;
}