XineramaGetImageData(DrawablePtr *pDrawables,
                     int left,
                     int top,
                     int width,
                     int height,
                     unsigned int format,
                     unsigned long planemask,
                     char *data, int pitch, Bool isRoot)
{
    RegionRec SrcRegion, ScreenRegion, GrabRegion;
    BoxRec SrcBox, *pbox;
    int x, y, w, h, i, j, nbox, size, sizeNeeded, ScratchPitch, inOut, depth;
    DrawablePtr pDraw = pDrawables[0];
    char *ScratchMem = NULL;

    size = 0;

     
    SrcBox.x1 = left;
    SrcBox.y1 = top;
    if (!isRoot) {
        SrcBox.x1 += pDraw->x + screenInfo.screens[0]->x;
        SrcBox.y1 += pDraw->y + screenInfo.screens[0]->y;
    }
    SrcBox.x2 = SrcBox.x1 + width;
    SrcBox.y2 = SrcBox.y1 + height;

    RegionInit(&SrcRegion, &SrcBox, 1);
    RegionNull(&GrabRegion);

    depth = (format == XYPixmap) ? 1 : pDraw->depth;

    FOR_NSCREENS(i) {
        BoxRec TheBox;
        ScreenPtr pScreen;

        pDraw = pDrawables[i];
        pScreen = pDraw->pScreen;

        TheBox.x1 = pScreen->x;
        TheBox.x2 = TheBox.x1 + pScreen->width;
        TheBox.y1 = pScreen->y;
        TheBox.y2 = TheBox.y1 + pScreen->height;

        RegionInit(&ScreenRegion, &TheBox, 1);
        inOut = RegionContainsRect(&ScreenRegion, &SrcBox);
        if (inOut == rgnPART)
            RegionIntersect(&GrabRegion, &SrcRegion, &ScreenRegion);
        RegionUninit(&ScreenRegion);

        if (inOut == rgnIN) {
            (*pScreen->GetImage) (pDraw,
                                  SrcBox.x1 - pDraw->x -
                                  screenInfo.screens[i]->x,
                                  SrcBox.y1 - pDraw->y -
                                  screenInfo.screens[i]->y, width, height,
                                  format, planemask, data);
            break;
        }
        else if (inOut == rgnOUT)
            continue;

        nbox = RegionNumRects(&GrabRegion);

        if (nbox) {
            pbox = RegionRects(&GrabRegion);

            while (nbox--) {
                w = pbox->x2 - pbox->x1;
                h = pbox->y2 - pbox->y1;
                ScratchPitch = PixmapBytePad(w, depth);
                sizeNeeded = ScratchPitch * h;

                if (sizeNeeded > size) {
                    char *tmpdata = ScratchMem;

                    ScratchMem = realloc(ScratchMem, sizeNeeded);
                    if (ScratchMem)
                        size = sizeNeeded;
                    else {
                        ScratchMem = tmpdata;
                        break;
                    }
                }

                x = pbox->x1 - pDraw->x - screenInfo.screens[i]->x;
                y = pbox->y1 - pDraw->y - screenInfo.screens[i]->y;

                (*pScreen->GetImage) (pDraw, x, y, w, h,
                                      format, planemask, ScratchMem);

                 

                if (depth == 1) {
                    int k, shift, leftover, index, index2;

                    x = pbox->x1 - SrcBox.x1;
                    y = pbox->y1 - SrcBox.y1;
                    shift = x & 7;
                    x >>= 3;
                    leftover = w & 7;
                    w >>= 3;

                     
                    if (leftover) {
                        int mask = (1 << leftover) - 1;

                        for (j = h, k = w; j--; k += ScratchPitch)
                            ScratchMem[k] &= mask;
                    }

                    for (j = 0, index = (pitch * y) + x, index2 = 0; j < h;
                         j++, index += pitch, index2 += ScratchPitch) {
                        if (w) {
                            if (!shift)
                                memcpy(data + index, ScratchMem + index2, w);
                            else
                                CopyBits(data + index, shift,
                                         ScratchMem + index2, w);
                        }

                        if (leftover) {
                            data[index + w] |=
                                SHIFT_L(ScratchMem[index2 + w], shift);
                            if ((shift + leftover) > 8)
                                data[index + w + 1] |=
                                    SHIFT_R(ScratchMem[index2 + w],
                                            (8 - shift));
                        }
                    }
                }
                else {
                    j = BitsPerPixel(depth) >> 3;
                    x = (pbox->x1 - SrcBox.x1) * j;
                    y = pbox->y1 - SrcBox.y1;
                    w *= j;

                    for (j = 0; j < h; j++) {
                        memcpy(data + (pitch * (y + j)) + x,
                               ScratchMem + (ScratchPitch * j), w);
                    }
                }
                pbox++;
            }

            RegionSubtract(&SrcRegion, &SrcRegion, &GrabRegion);
            if (!RegionNotEmpty(&SrcRegion))
                break;
        }

    }

    free(ScratchMem);

    RegionUninit(&SrcRegion);
    RegionUninit(&GrabRegion);
}
