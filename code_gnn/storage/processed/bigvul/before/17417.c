ScreenSaverFreeAttr(void *value, XID id)
{
    ScreenSaverAttrPtr pOldAttr = (ScreenSaverAttrPtr) value;
    ScreenPtr pScreen = pOldAttr->screen;

    SetupScreen(pScreen);

    if (!pPriv)
        return TRUE;
    if (pPriv->attr != pOldAttr)
        return TRUE;
    FreeScreenAttr(pOldAttr);
    pPriv->attr = NULL;
    if (pPriv->hasWindow) {
        dixSaveScreens(serverClient, SCREEN_SAVER_FORCER, ScreenSaverReset);
        dixSaveScreens(serverClient, SCREEN_SAVER_FORCER, ScreenSaverActive);
    }
    CheckScreenPrivate(pScreen);
    return TRUE;
}
