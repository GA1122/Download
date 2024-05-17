DGACopyModeInfo(DGAModePtr mode, XDGAModePtr xmode)
{
    DisplayModePtr dmode = mode->mode;

    xmode->num = mode->num;
    xmode->name = dmode->name;
    xmode->VSync_num = (int) (dmode->VRefresh * 1000.0);
    xmode->VSync_den = 1000;
    xmode->flags = mode->flags;
    xmode->imageWidth = mode->imageWidth;
    xmode->imageHeight = mode->imageHeight;
    xmode->pixmapWidth = mode->pixmapWidth;
    xmode->pixmapHeight = mode->pixmapHeight;
    xmode->bytesPerScanline = mode->bytesPerScanline;
    xmode->byteOrder = mode->byteOrder;
    xmode->depth = mode->depth;
    xmode->bitsPerPixel = mode->bitsPerPixel;
    xmode->red_mask = mode->red_mask;
    xmode->green_mask = mode->green_mask;
    xmode->blue_mask = mode->blue_mask;
    xmode->visualClass = mode->visualClass;
    xmode->viewportWidth = mode->viewportWidth;
    xmode->viewportHeight = mode->viewportHeight;
    xmode->xViewportStep = mode->xViewportStep;
    xmode->yViewportStep = mode->yViewportStep;
    xmode->maxViewportX = mode->maxViewportX;
    xmode->maxViewportY = mode->maxViewportY;
    xmode->viewportFlags = mode->viewportFlags;
    xmode->reserved1 = mode->reserved1;
    xmode->reserved2 = mode->reserved2;
    xmode->offset = mode->offset;

    if (dmode->Flags & V_INTERLACE)
        xmode->flags |= DGA_INTERLACED;
    if (dmode->Flags & V_DBLSCAN)
        xmode->flags |= DGA_DOUBLESCAN;
}
