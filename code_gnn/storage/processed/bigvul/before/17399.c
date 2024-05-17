DestroySaverWindow(ScreenPtr pScreen)
{
    SetupScreen(pScreen);
    ScreenSaverStuffPtr pSaver;

    if (!pPriv || !pPriv->hasWindow)
        return FALSE;

    pSaver = &pScreen->screensaver;
    if (pSaver->pWindow) {
        pSaver->pWindow = NullWindow;
        FreeResource(pSaver->wid, RT_NONE);
    }
    pPriv->hasWindow = FALSE;
    CheckScreenPrivate(pScreen);
    UninstallSaverColormap(pScreen);
    return TRUE;
}
