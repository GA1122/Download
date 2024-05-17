PanoramiXCreateConnectionBlock(void)
{
    int i, j, length;
    Bool disable_backing_store = FALSE;
    int old_width, old_height;
    float width_mult, height_mult;
    xWindowRoot *root;
    xVisualType *visual;
    xDepth *depth;
    VisualPtr pVisual;
    ScreenPtr pScreen;

     

    if (!PanoramiXNumDepths) {
        ErrorF("Xinerama error: No common visuals\n");
        return FALSE;
    }

    for (i = 1; i < screenInfo.numScreens; i++) {
        pScreen = screenInfo.screens[i];
        if (pScreen->rootDepth != screenInfo.screens[0]->rootDepth) {
            ErrorF("Xinerama error: Root window depths differ\n");
            return FALSE;
        }
        if (pScreen->backingStoreSupport !=
            screenInfo.screens[0]->backingStoreSupport)
            disable_backing_store = TRUE;
    }

    if (disable_backing_store) {
        for (i = 0; i < screenInfo.numScreens; i++) {
            pScreen = screenInfo.screens[i];
            pScreen->backingStoreSupport = NotUseful;
        }
    }

    i = screenInfo.numScreens;
    screenInfo.numScreens = 1;
    if (!CreateConnectionBlock()) {
        screenInfo.numScreens = i;
        return FALSE;
    }

    screenInfo.numScreens = i;

    root = (xWindowRoot *) (ConnectionInfo + connBlockScreenStart);
    length = connBlockScreenStart + sizeof(xWindowRoot);

     
    root->nDepths = PanoramiXNumDepths;

    for (i = 0; i < PanoramiXNumDepths; i++) {
        depth = (xDepth *) (ConnectionInfo + length);
        depth->depth = PanoramiXDepths[i].depth;
        depth->nVisuals = PanoramiXDepths[i].numVids;
        length += sizeof(xDepth);
        visual = (xVisualType *) (ConnectionInfo + length);

        for (j = 0; j < depth->nVisuals; j++, visual++) {
            visual->visualID = PanoramiXDepths[i].vids[j];

            for (pVisual = PanoramiXVisuals;
                 pVisual->vid != visual->visualID; pVisual++);

            visual->class = pVisual->class;
            visual->bitsPerRGB = pVisual->bitsPerRGBValue;
            visual->colormapEntries = pVisual->ColormapEntries;
            visual->redMask = pVisual->redMask;
            visual->greenMask = pVisual->greenMask;
            visual->blueMask = pVisual->blueMask;
        }

        length += (depth->nVisuals * sizeof(xVisualType));
    }

    connSetupPrefix.length = bytes_to_int32(length);

    for (i = 0; i < PanoramiXNumDepths; i++)
        free(PanoramiXDepths[i].vids);
    free(PanoramiXDepths);
    PanoramiXDepths = NULL;

     

    old_width = root->pixWidth;
    old_height = root->pixHeight;

    root->pixWidth = PanoramiXPixWidth;
    root->pixHeight = PanoramiXPixHeight;
    width_mult = (1.0 * root->pixWidth) / old_width;
    height_mult = (1.0 * root->pixHeight) / old_height;
    root->mmWidth *= width_mult;
    root->mmHeight *= height_mult;

    while (ConnectionCallbackList) {
        void *tmp;

        tmp = (void *) ConnectionCallbackList;
        (*ConnectionCallbackList->func) ();
        ConnectionCallbackList = ConnectionCallbackList->next;
        free(tmp);
    }

    return TRUE;
}
