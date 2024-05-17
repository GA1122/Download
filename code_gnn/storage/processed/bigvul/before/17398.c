CreateSaverWindow(ScreenPtr pScreen)
{
    SetupScreen(pScreen);
    ScreenSaverStuffPtr pSaver;
    ScreenSaverAttrPtr pAttr;
    WindowPtr pWin;
    int result;
    unsigned long mask;
    Colormap wantMap;
    ColormapPtr pCmap;

    pSaver = &pScreen->screensaver;
    if (pSaver->pWindow) {
        pSaver->pWindow = NullWindow;
        FreeResource(pSaver->wid, RT_NONE);
        if (pPriv) {
            UninstallSaverColormap(pScreen);
            pPriv->hasWindow = FALSE;
            CheckScreenPrivate(pScreen);
        }
    }

    if (!pPriv || !(pAttr = pPriv->attr))
        return FALSE;

    pPriv->installedMap = None;

    if (GrabInProgress && GrabInProgress != pAttr->client->index)
        return FALSE;

    pWin = CreateWindow(pSaver->wid, pScreen->root,
                        pAttr->x, pAttr->y, pAttr->width, pAttr->height,
                        pAttr->borderWidth, pAttr->class,
                        pAttr->mask, (XID *) pAttr->values,
                        pAttr->depth, serverClient, pAttr->visual, &result);
    if (!pWin)
        return FALSE;

    if (!AddResource(pWin->drawable.id, RT_WINDOW, pWin))
        return FALSE;

    mask = 0;
    if (pAttr->pBackgroundPixmap) {
        pWin->backgroundState = BackgroundPixmap;
        pWin->background.pixmap = pAttr->pBackgroundPixmap;
        pAttr->pBackgroundPixmap->refcnt++;
        mask |= CWBackPixmap;
    }
    if (pAttr->pBorderPixmap) {
        pWin->borderIsPixel = FALSE;
        pWin->border.pixmap = pAttr->pBorderPixmap;
        pAttr->pBorderPixmap->refcnt++;
        mask |= CWBorderPixmap;
    }
    if (pAttr->pCursor) {
        CursorPtr cursor;
        if (!pWin->optional)
            if (!MakeWindowOptional(pWin)) {
                FreeResource(pWin->drawable.id, RT_NONE);
                return FALSE;
            }
        cursor = RefCursor(pAttr->pCursor);
        if (pWin->optional->cursor)
            FreeCursor(pWin->optional->cursor, (Cursor) 0);
        pWin->optional->cursor = cursor;
        pWin->cursorIsNone = FALSE;
        CheckWindowOptionalNeed(pWin);
        mask |= CWCursor;
    }
    if (mask)
        (*pScreen->ChangeWindowAttributes) (pWin, mask);

    if (pAttr->colormap != None)
        (void) ChangeWindowAttributes(pWin, CWColormap, &pAttr->colormap,
                                      serverClient);

    MapWindow(pWin, serverClient);

    pPriv->hasWindow = TRUE;
    pSaver->pWindow = pWin;

     
    wantMap = wColormap(pWin);
    if (wantMap == None || IsMapInstalled(wantMap, pWin))
        return TRUE;

    result = dixLookupResourceByType((void **) &pCmap, wantMap, RT_COLORMAP,
                                     serverClient, DixInstallAccess);
    if (result != Success)
        return TRUE;

    pPriv->installedMap = wantMap;

    (*pCmap->pScreen->InstallColormap) (pCmap);

    return TRUE;
}
