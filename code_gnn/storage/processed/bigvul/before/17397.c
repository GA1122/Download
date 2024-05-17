CheckScreenPrivate(ScreenPtr pScreen)
{
    SetupScreen(pScreen);

    if (!pPriv)
        return;
    if (!pPriv->attr && !pPriv->events &&
        !pPriv->hasWindow && pPriv->installedMap == None) {
        free(pPriv);
        SetScreenPrivate(pScreen, NULL);
        pScreen->screensaver.ExternalScreenSaver = NULL;
    }
}
