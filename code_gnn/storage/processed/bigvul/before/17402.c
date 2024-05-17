MakeScreenPrivate(ScreenPtr pScreen)
{
    SetupScreen(pScreen);

    if (pPriv)
        return pPriv;
    pPriv = New(ScreenSaverScreenPrivateRec);
    if (!pPriv)
        return 0;
    pPriv->events = 0;
    pPriv->attr = 0;
    pPriv->hasWindow = FALSE;
    pPriv->installedMap = None;
    SetScreenPrivate(pScreen, pPriv);
    pScreen->screensaver.ExternalScreenSaver = ScreenSaverHandle;
    return pPriv;
}
