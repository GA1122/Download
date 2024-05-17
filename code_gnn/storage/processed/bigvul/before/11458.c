fbFetch_r8g8b8 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD8 *pixel = (const CARD8 *)bits + 3*x;
    const CARD8 *end = pixel + 3*width;
    while (pixel < end) {
        CARD32 b = Fetch24(pixel) | 0xff000000;
        pixel += 3;
        WRITE(buffer++, b);
    }
}
