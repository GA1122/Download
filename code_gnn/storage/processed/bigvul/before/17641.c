CopyCursorToImage(CursorPtr pCursor, CARD32 *image)
{
    int width = pCursor->bits->width;
    int height = pCursor->bits->height;
    int npixels = width * height;

    if (pCursor->bits->argb)
        memcpy(image, pCursor->bits->argb, npixels * sizeof(CARD32));
    else
    {
        unsigned char *srcLine = pCursor->bits->source;
        unsigned char *mskLine = pCursor->bits->mask;
        int stride = BitmapBytePad(width);
        int x, y;
        CARD32 fg, bg;

        fg = (0xff000000 |
              ((pCursor->foreRed & 0xff00) << 8) |
              (pCursor->foreGreen & 0xff00) | (pCursor->foreBlue >> 8));
        bg = (0xff000000 |
              ((pCursor->backRed & 0xff00) << 8) |
              (pCursor->backGreen & 0xff00) | (pCursor->backBlue >> 8));
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                if (GetBit(mskLine, x)) {
                    if (GetBit(srcLine, x))
                        *image++ = fg;
                    else
                        *image++ = bg;
                }
                else
                    *image++ = 0;
            }
            srcLine += stride;
            mskLine += stride;
        }
    }
}
