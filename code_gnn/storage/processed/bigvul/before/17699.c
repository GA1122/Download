DGAChangePixmapMode(int index, int *x, int *y, int mode)
{
    DGAScreenPtr pScreenPriv;
    DGADevicePtr pDev;
    DGAModePtr pMode;
    PixmapPtr pPix;

    if (!DGAScreenKeyRegistered)
        return FALSE;

    pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

    if (!pScreenPriv || !pScreenPriv->current || !pScreenPriv->current->pPix)
        return FALSE;

    pDev = pScreenPriv->current;
    pPix = pDev->pPix;
    pMode = pDev->mode;

    if (mode) {
        int shift = 2;

        if (*x > (pMode->pixmapWidth - pMode->viewportWidth))
            *x = pMode->pixmapWidth - pMode->viewportWidth;
        if (*y > (pMode->pixmapHeight - pMode->viewportHeight))
            *y = pMode->pixmapHeight - pMode->viewportHeight;

        switch (xf86Screens[index]->bitsPerPixel) {
        case 16:
            shift = 1;
            break;
        case 32:
            shift = 0;
            break;
        default:
            break;
        }

        if (BITMAP_SCANLINE_PAD == 64)
            shift++;

        *x = (*x >> shift) << shift;

        pPix->drawable.x = *x;
        pPix->drawable.y = *y;
        pPix->drawable.width = pMode->viewportWidth;
        pPix->drawable.height = pMode->viewportHeight;
    }
    else {
        pPix->drawable.x = 0;
        pPix->drawable.y = 0;
        pPix->drawable.width = pMode->pixmapWidth;
        pPix->drawable.height = pMode->pixmapHeight;
    }
    pPix->drawable.serialNumber = NEXT_SERIAL_NUMBER;
    pScreenPriv->pixmapMode = mode;

    return TRUE;
}
