PanoramiXExtensionInit(void)
{
    int i;
    Bool success = FALSE;
    ExtensionEntry *extEntry;
    ScreenPtr pScreen = screenInfo.screens[0];
    PanoramiXScreenPtr pScreenPriv;

    if (noPanoramiXExtension)
        return;

    if (!dixRegisterPrivateKey(&PanoramiXScreenKeyRec, PRIVATE_SCREEN, 0)) {
        noPanoramiXExtension = TRUE;
        return;
    }

    if (!dixRegisterPrivateKey
        (&PanoramiXGCKeyRec, PRIVATE_GC, sizeof(PanoramiXGCRec))) {
        noPanoramiXExtension = TRUE;
        return;
    }

    PanoramiXNumScreens = screenInfo.numScreens;
    if (PanoramiXNumScreens == 1) {      
        noPanoramiXExtension = TRUE;
        return;
    }

    while (panoramiXGeneration != serverGeneration) {
        extEntry = AddExtension(PANORAMIX_PROTOCOL_NAME, 0, 0,
                                ProcPanoramiXDispatch,
                                SProcPanoramiXDispatch, PanoramiXResetProc,
                                StandardMinorOpcode);
        if (!extEntry)
            break;

         

        FOR_NSCREENS(i) {
            pScreen = screenInfo.screens[i];
            pScreenPriv = malloc(sizeof(PanoramiXScreenRec));
            dixSetPrivate(&pScreen->devPrivates, PanoramiXScreenKey,
                          pScreenPriv);
            if (!pScreenPriv) {
                noPanoramiXExtension = TRUE;
                return;
            }

            pScreenPriv->CreateGC = pScreen->CreateGC;
            pScreenPriv->CloseScreen = pScreen->CloseScreen;

            pScreen->CreateGC = XineramaCreateGC;
            pScreen->CloseScreen = XineramaCloseScreen;
        }

        XRC_DRAWABLE = CreateNewResourceClass();
        XRT_WINDOW = CreateNewResourceType(XineramaDeleteResource,
                                           "XineramaWindow");
        if (XRT_WINDOW)
            XRT_WINDOW |= XRC_DRAWABLE;
        XRT_PIXMAP = CreateNewResourceType(XineramaDeleteResource,
                                           "XineramaPixmap");
        if (XRT_PIXMAP)
            XRT_PIXMAP |= XRC_DRAWABLE;
        XRT_GC = CreateNewResourceType(XineramaDeleteResource, "XineramaGC");
        XRT_COLORMAP = CreateNewResourceType(XineramaDeleteResource,
                                             "XineramaColormap");

        if (XRT_WINDOW && XRT_PIXMAP && XRT_GC && XRT_COLORMAP) {
            panoramiXGeneration = serverGeneration;
            success = TRUE;
        }
        SetResourceTypeErrorValue(XRT_WINDOW, BadWindow);
        SetResourceTypeErrorValue(XRT_PIXMAP, BadPixmap);
        SetResourceTypeErrorValue(XRT_GC, BadGC);
        SetResourceTypeErrorValue(XRT_COLORMAP, BadColor);
    }

    if (!success) {
        noPanoramiXExtension = TRUE;
        ErrorF(PANORAMIX_PROTOCOL_NAME " extension failed to initialize\n");
        return;
    }

    XineramaInitData();

     

    for (i = 256; i--;)
        SavedProcVector[i] = ProcVector[i];

    ProcVector[X_CreateWindow] = PanoramiXCreateWindow;
    ProcVector[X_ChangeWindowAttributes] = PanoramiXChangeWindowAttributes;
    ProcVector[X_DestroyWindow] = PanoramiXDestroyWindow;
    ProcVector[X_DestroySubwindows] = PanoramiXDestroySubwindows;
    ProcVector[X_ChangeSaveSet] = PanoramiXChangeSaveSet;
    ProcVector[X_ReparentWindow] = PanoramiXReparentWindow;
    ProcVector[X_MapWindow] = PanoramiXMapWindow;
    ProcVector[X_MapSubwindows] = PanoramiXMapSubwindows;
    ProcVector[X_UnmapWindow] = PanoramiXUnmapWindow;
    ProcVector[X_UnmapSubwindows] = PanoramiXUnmapSubwindows;
    ProcVector[X_ConfigureWindow] = PanoramiXConfigureWindow;
    ProcVector[X_CirculateWindow] = PanoramiXCirculateWindow;
    ProcVector[X_GetGeometry] = PanoramiXGetGeometry;
    ProcVector[X_TranslateCoords] = PanoramiXTranslateCoords;
    ProcVector[X_CreatePixmap] = PanoramiXCreatePixmap;
    ProcVector[X_FreePixmap] = PanoramiXFreePixmap;
    ProcVector[X_CreateGC] = PanoramiXCreateGC;
    ProcVector[X_ChangeGC] = PanoramiXChangeGC;
    ProcVector[X_CopyGC] = PanoramiXCopyGC;
    ProcVector[X_SetDashes] = PanoramiXSetDashes;
    ProcVector[X_SetClipRectangles] = PanoramiXSetClipRectangles;
    ProcVector[X_FreeGC] = PanoramiXFreeGC;
    ProcVector[X_ClearArea] = PanoramiXClearToBackground;
    ProcVector[X_CopyArea] = PanoramiXCopyArea;
    ProcVector[X_CopyPlane] = PanoramiXCopyPlane;
    ProcVector[X_PolyPoint] = PanoramiXPolyPoint;
    ProcVector[X_PolyLine] = PanoramiXPolyLine;
    ProcVector[X_PolySegment] = PanoramiXPolySegment;
    ProcVector[X_PolyRectangle] = PanoramiXPolyRectangle;
    ProcVector[X_PolyArc] = PanoramiXPolyArc;
    ProcVector[X_FillPoly] = PanoramiXFillPoly;
    ProcVector[X_PolyFillRectangle] = PanoramiXPolyFillRectangle;
    ProcVector[X_PolyFillArc] = PanoramiXPolyFillArc;
    ProcVector[X_PutImage] = PanoramiXPutImage;
    ProcVector[X_GetImage] = PanoramiXGetImage;
    ProcVector[X_PolyText8] = PanoramiXPolyText8;
    ProcVector[X_PolyText16] = PanoramiXPolyText16;
    ProcVector[X_ImageText8] = PanoramiXImageText8;
    ProcVector[X_ImageText16] = PanoramiXImageText16;
    ProcVector[X_CreateColormap] = PanoramiXCreateColormap;
    ProcVector[X_FreeColormap] = PanoramiXFreeColormap;
    ProcVector[X_CopyColormapAndFree] = PanoramiXCopyColormapAndFree;
    ProcVector[X_InstallColormap] = PanoramiXInstallColormap;
    ProcVector[X_UninstallColormap] = PanoramiXUninstallColormap;
    ProcVector[X_AllocColor] = PanoramiXAllocColor;
    ProcVector[X_AllocNamedColor] = PanoramiXAllocNamedColor;
    ProcVector[X_AllocColorCells] = PanoramiXAllocColorCells;
    ProcVector[X_AllocColorPlanes] = PanoramiXAllocColorPlanes;
    ProcVector[X_FreeColors] = PanoramiXFreeColors;
    ProcVector[X_StoreColors] = PanoramiXStoreColors;
    ProcVector[X_StoreNamedColor] = PanoramiXStoreNamedColor;

    PanoramiXRenderInit();
    PanoramiXFixesInit();
    PanoramiXDamageInit();
#ifdef COMPOSITE
    PanoramiXCompositeInit();
#endif

}
