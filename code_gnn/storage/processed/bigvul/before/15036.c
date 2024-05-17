static int init_screen(ScreenPtr pScreen, int i, Bool gpu)
{
    int scanlinepad, format, depth, bitsPerPixel, j, k;

    dixInitScreenSpecificPrivates(pScreen);

    if (!dixAllocatePrivates(&pScreen->devPrivates, PRIVATE_SCREEN)) {
        return -1;
    }
    pScreen->myNum = i;
    if (gpu) {
        pScreen->myNum += GPU_SCREEN_OFFSET;
        pScreen->isGPU = TRUE;
    }
    pScreen->totalPixmapSize = 0;        
    pScreen->ClipNotify = 0;     
    pScreen->CreateScreenResources = 0;

    xorg_list_init(&pScreen->pixmap_dirty_list);
    xorg_list_init(&pScreen->unattached_list);
    xorg_list_init(&pScreen->output_slave_list);
    xorg_list_init(&pScreen->offload_slave_list);

     
    for (format = 0; format < screenInfo.numPixmapFormats; format++) {
        depth = screenInfo.formats[format].depth;
        bitsPerPixel = screenInfo.formats[format].bitsPerPixel;
        scanlinepad = screenInfo.formats[format].scanlinePad;
        j = indexForBitsPerPixel[bitsPerPixel];
        k = indexForScanlinePad[scanlinepad];
        PixmapWidthPaddingInfo[depth].padPixelsLog2 = answer[j][k];
        PixmapWidthPaddingInfo[depth].padRoundUp =
            (scanlinepad / bitsPerPixel) - 1;
        j = indexForBitsPerPixel[8];     
        PixmapWidthPaddingInfo[depth].padBytesLog2 = answer[j][k];
        PixmapWidthPaddingInfo[depth].bitsPerPixel = bitsPerPixel;
        if (answerBytesPerPixel[bitsPerPixel]) {
            PixmapWidthPaddingInfo[depth].notPower2 = 1;
            PixmapWidthPaddingInfo[depth].bytesPerPixel =
                answerBytesPerPixel[bitsPerPixel];
        }
        else {
            PixmapWidthPaddingInfo[depth].notPower2 = 0;
        }
    }
    return 0;
}
