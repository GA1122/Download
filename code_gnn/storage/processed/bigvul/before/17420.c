ScreenSaverHandle(ScreenPtr pScreen, int xstate, Bool force)
{
    int state = 0;
    Bool ret = FALSE;
    ScreenSaverScreenPrivatePtr pPriv;

    switch (xstate) {
    case SCREEN_SAVER_ON:
        state = ScreenSaverOn;
        ret = CreateSaverWindow(pScreen);
        break;
    case SCREEN_SAVER_OFF:
        state = ScreenSaverOff;
        ret = DestroySaverWindow(pScreen);
        break;
    case SCREEN_SAVER_CYCLE:
        state = ScreenSaverCycle;
        pPriv = GetScreenPrivate(pScreen);
        if (pPriv && pPriv->hasWindow)
            ret = TRUE;

    }
#ifdef PANORAMIX
    if (noPanoramiXExtension || !pScreen->myNum)
#endif
        SendScreenSaverNotify(pScreen, state, force);
    return ret;
}
