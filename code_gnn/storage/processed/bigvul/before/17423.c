SendScreenSaverNotify(ScreenPtr pScreen, int state, Bool forced)
{
    ScreenSaverScreenPrivatePtr pPriv;
    ScreenSaverEventPtr pEv;
    unsigned long mask;
    int kind;

    UpdateCurrentTimeIf();
    mask = ScreenSaverNotifyMask;
    if (state == ScreenSaverCycle)
        mask = ScreenSaverCycleMask;
    pScreen = screenInfo.screens[pScreen->myNum];
    pPriv = GetScreenPrivate(pScreen);
    if (!pPriv)
        return;
    if (pPriv->attr)
        kind = ScreenSaverExternal;
    else if (ScreenSaverBlanking != DontPreferBlanking)
        kind = ScreenSaverBlanked;
    else
        kind = ScreenSaverInternal;
    for (pEv = pPriv->events; pEv; pEv = pEv->next) {
        if (pEv->mask & mask) {
            xScreenSaverNotifyEvent ev = {
                .type = ScreenSaverNotify + ScreenSaverEventBase,
                .state = state,
                .timestamp = currentTime.milliseconds,
                .root = pScreen->root->drawable.id,
                .window = pScreen->screensaver.wid,
                .kind = kind,
                .forced = forced
            };
            WriteEventsToClient(pEv->client, 1, (xEvent *) &ev);
        }
    }
}
