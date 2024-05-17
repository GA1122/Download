CreateConnectionBlock(void)
{
    xConnSetup setup;
    xWindowRoot root;
    xDepth depth;
    xVisualType visual;
    xPixmapFormat format;
    unsigned long vid;
    int i, j, k, lenofblock, sizesofar = 0;
    char *pBuf;

    memset(&setup, 0, sizeof(xConnSetup));
     

    setup.release = VendorRelease;
     
    setup.imageByteOrder = screenInfo.imageByteOrder;

    setup.bitmapScanlineUnit = screenInfo.bitmapScanlineUnit;
    setup.bitmapScanlinePad = screenInfo.bitmapScanlinePad;

    setup.bitmapBitOrder = screenInfo.bitmapBitOrder;
    setup.motionBufferSize = NumMotionEvents();
    setup.numRoots = screenInfo.numScreens;
    setup.nbytesVendor = strlen(VendorString);
    setup.numFormats = screenInfo.numPixmapFormats;
    setup.maxRequestSize = MAX_REQUEST_SIZE;
    QueryMinMaxKeyCodes(&setup.minKeyCode, &setup.maxKeyCode);

    lenofblock = sizeof(xConnSetup) +
        pad_to_int32(setup.nbytesVendor) +
        (setup.numFormats * sizeof(xPixmapFormat)) +
        (setup.numRoots * sizeof(xWindowRoot));
    ConnectionInfo = malloc(lenofblock);
    if (!ConnectionInfo)
        return FALSE;

    memmove(ConnectionInfo, (char *) &setup, sizeof(xConnSetup));
    sizesofar = sizeof(xConnSetup);
    pBuf = ConnectionInfo + sizeof(xConnSetup);

    memmove(pBuf, VendorString, (int) setup.nbytesVendor);
    sizesofar += setup.nbytesVendor;
    pBuf += setup.nbytesVendor;
    i = padding_for_int32(setup.nbytesVendor);
    sizesofar += i;
    while (--i >= 0)
        *pBuf++ = 0;

    memset(&format, 0, sizeof(xPixmapFormat));
    for (i = 0; i < screenInfo.numPixmapFormats; i++) {
        format.depth = screenInfo.formats[i].depth;
        format.bitsPerPixel = screenInfo.formats[i].bitsPerPixel;
        format.scanLinePad = screenInfo.formats[i].scanlinePad;
        memmove(pBuf, (char *) &format, sizeof(xPixmapFormat));
        pBuf += sizeof(xPixmapFormat);
        sizesofar += sizeof(xPixmapFormat);
    }

    connBlockScreenStart = sizesofar;
    memset(&depth, 0, sizeof(xDepth));
    memset(&visual, 0, sizeof(xVisualType));
    for (i = 0; i < screenInfo.numScreens; i++) {
        ScreenPtr pScreen;
        DepthPtr pDepth;
        VisualPtr pVisual;

        pScreen = screenInfo.screens[i];
        root.windowId = pScreen->root->drawable.id;
        root.defaultColormap = pScreen->defColormap;
        root.whitePixel = pScreen->whitePixel;
        root.blackPixel = pScreen->blackPixel;
        root.currentInputMask = 0;       
        root.pixWidth = pScreen->width;
        root.pixHeight = pScreen->height;
        root.mmWidth = pScreen->mmWidth;
        root.mmHeight = pScreen->mmHeight;
        root.minInstalledMaps = pScreen->minInstalledCmaps;
        root.maxInstalledMaps = pScreen->maxInstalledCmaps;
        root.rootVisualID = pScreen->rootVisual;
        root.backingStore = pScreen->backingStoreSupport;
        root.saveUnders = FALSE;
        root.rootDepth = pScreen->rootDepth;
        root.nDepths = pScreen->numDepths;
        memmove(pBuf, (char *) &root, sizeof(xWindowRoot));
        sizesofar += sizeof(xWindowRoot);
        pBuf += sizeof(xWindowRoot);

        pDepth = pScreen->allowedDepths;
        for (j = 0; j < pScreen->numDepths; j++, pDepth++) {
            lenofblock += sizeof(xDepth) +
                (pDepth->numVids * sizeof(xVisualType));
            pBuf = (char *) realloc(ConnectionInfo, lenofblock);
            if (!pBuf) {
                free(ConnectionInfo);
                return FALSE;
            }
            ConnectionInfo = pBuf;
            pBuf += sizesofar;
            depth.depth = pDepth->depth;
            depth.nVisuals = pDepth->numVids;
            memmove(pBuf, (char *) &depth, sizeof(xDepth));
            pBuf += sizeof(xDepth);
            sizesofar += sizeof(xDepth);
            for (k = 0; k < pDepth->numVids; k++) {
                vid = pDepth->vids[k];
                for (pVisual = pScreen->visuals;
                     pVisual->vid != vid; pVisual++);
                visual.visualID = vid;
                visual.class = pVisual->class;
                visual.bitsPerRGB = pVisual->bitsPerRGBValue;
                visual.colormapEntries = pVisual->ColormapEntries;
                visual.redMask = pVisual->redMask;
                visual.greenMask = pVisual->greenMask;
                visual.blueMask = pVisual->blueMask;
                memmove(pBuf, (char *) &visual, sizeof(xVisualType));
                pBuf += sizeof(xVisualType);
                sizesofar += sizeof(xVisualType);
            }
        }
    }
    connSetupPrefix.success = xTrue;
    connSetupPrefix.length = lenofblock / 4;
    connSetupPrefix.majorVersion = X_PROTOCOL;
    connSetupPrefix.minorVersion = X_PROTOCOL_REVISION;
    return TRUE;
}
